


char* idealand_file_exe_path()
{
  char *r=NULL; int got=0;
  #ifdef _MSC_VER
    got=(_get_pgmptr(&r)==0); 
  #elif __GNUC__
    r=(char *)idealand_malloc(IdealandMaxPathLen); if(r==NULL) return NULL;
    got=readlink("/proc/self/exe", r, IdealandMaxPathLen-1);
    if(got>0) r[got]=0; else got=0;
  #endif
  if(got) {idealand_log("exe file path = %s\n", r); return r;} 
  else {idealand_log("cannot get exe path"); return NULL;}
}



char* idealand_file_exe_dir()
{
  char* r = idealand_file_exe_path();  if (r == NULL) return NULL;
  INT64 rindex = idealand_string_rindex('\\', r); if (rindex < 0) rindex = idealand_string_rindex('/', r);
  if (rindex < 0) { idealand_log("could not find dir sep in exe path in %s.", __func__); return NULL; }
  r[rindex+1] = 0; idealand_log("exe dir = %s\n", r); return r;
}




int idealand_file_exist(char* path, int file_dir)
{
  int r = 0;
  if ((r = idealand_check_path(path, "path", __func__)) < 0) { return r; }
  if ((r = idealand_check_int(file_dir, "file_dir", __func__, 3, 1)) < 0) { return r; }

  struct stat status; r=stat(path, &status); if(r!=0) return -1;
  if ((file_dir == 2 || file_dir == 3) && (status.st_mode & S_IFDIR)) return 0;
  if ((file_dir == 1 || file_dir == 3) && (S_IFREG & status.st_mode)) return 0;
  return -1;
}



INT64 idealand_get_file_info(char* collection, char* name_start, IdealandFileInfo* pf, int print)
{
  int r = 0;
  if ((r = idealand_check_filename(collection, "collection", __func__)) < 0) { return r; }
  if ((r = idealand_check_filename(name_start, "name_start", __func__)) < 0) { return r; }
  if ((r = idealand_check_pointer(pf, "pf", __func__)) < 0) { return r; }


#ifdef _MSC_VER
  char* pattern = idealand_string(IdealandMaxPathLen - 1, NULL, "%s/%s*", collection, name_start); if (pattern == NULL) return -1;
  intptr_t fHandle;  IdealandFd fd;
  if ((fHandle = _findfirst(pattern, &fd)) == -1) { if (print) idealand_log("cannot find file: %s\n", pattern); return -1; }
  _findclose(fHandle);  
  pf->size = fd.size; memcpy(pf->name, fd.name, strlen(fd.name)+1); pf->isdir = 0; 
  if (print) idealand_log("found file %s\n", fd.name);
  return pf->size;
  //do {
  //  printf("找到文件:%s,文件大小：%d\n", fileinfo.name, fileinfo.size);
  //} while (_findnext(fHandle, &fileinfo) == 0);
#elif __GNUC__
  int got = 0; struct dirent* pent; DIR* pd = opendir(collection);
  if (!pd) { idealand_log("open collection(%s) failed", collection); return -1; }
  while (pent = readdir(pd))
  {
    if (pent->d_type == DT_DIR || strcmp(pent->d_name, name_start)) { continue; }  else { got = 1;  break; }
  }
  closedir(pd);

  if (got)
  {
    char* path = idealand_string(IdealandMaxPathLen - 1, NULL, "%s/%s", collection, pent->d_name); if (path == NULL) return -1;
    struct stat s; r = stat(path, &s); if (r != 0) return -1;
    if (S_IFREG & s.st_mode) { strcpy(pf->name, pent->d_name); pf->size = s.st_size; pf->isdir = (s.st_mode & S_IFDIR); return pf->size; } 
  }
  return -1;
#endif
}
INT64 idealand_get_file_info(char* collection, int no, IdealandFileInfo* pf, int print)
{
  char name_start[5]; sprintf_s(name_start, 5, "%04d", no); name_start[4]=0;
  return idealand_get_file_info(collection, name_start, pf, print); 
}





int idealand_file_change_work_dir(char* path)
{
  int r = 0;
  if ((r = idealand_file_exist(path,2)) < 0) return r;

  if (_chdir(path) != 0) { idealand_log("change work dir to %s fail.", path); return -1; }
  idealand_log("change work dir to %s succeed.\n", path); return 0;
}




char * idealand_file_mkdir(char* name, char* parent)
{
  int r = 0; char* path=NULL;

  if ((r = idealand_check_filename(name, (char*)"name", __func__)) < 0) { return NULL; }
  char* parent2=NULL; if (parent != NULL && (parent2 = idealand_string_normalize_path(parent, 1)) == NULL) return NULL;
  if (parent2!=NULL && (r = idealand_file_exist(parent2, 2)) < 0) { idealand_log("parent dir(%s) do not exists, cannot creat dir in it.\n", parent2); r = -1; goto free1; }

  if(parent2 != NULL) path = idealand_string(2048, NULL, (char*)"%s%s%c", parent2, name, IdealandPathSep);
  else path = idealand_string(2048, NULL, (char*)"%s%c", name, IdealandPathSep);
  
  if (path == NULL) { r = -1; goto free1; }
  if (idealand_file_exist(path, 2) >= 0) {  /* printf("dir(%s) exists, no need to create.\n", path); */  goto free1; }
  
#ifdef _MSC_VER
  r=_mkdir(path);
#elif __GNUC__
  r=mkdir(path, 0700); // owner can read, write and execute
#endif
  if (r != 0) { idealand_log("create dir %s fail.", path); r = -1;  goto free2; }
  else { idealand_log("create dir %s succeed.\n", path); goto free1; }

free2: free(path); path = NULL;
free1:  if (parent2 != parent && parent2 != NULL) free(parent2);
  if (r >= 0) return path; else return NULL;
}


int idealand_file_create(char* path, int allow_exists, int utf8_head, FILE** ppf, const char *mode)
{
  if (idealand_check_pointer(path, "path", __func__) < 0) { return -1; }

  if (idealand_file_exist(path, 2) >= 0) { idealand_log("could not create file as the directory with same name exists: %s", path); return -1; }
  if (idealand_file_exist(path, 1) >= 0 && !allow_exists)
  { idealand_log("could not create file as the file with same name exists: %s", path); return -1; }

  int r = 0; FILE** ppf2=NULL;  if (ppf == NULL) { FILE* pf = NULL; ppf2 = &pf; }  else { ppf2 = ppf; }
  if (fopen_s(ppf2, path, "ab") != 0) { idealand_log("could not open file (%s) by mode ab.", path); r = -1; goto free1; }  fclose(*ppf2); *ppf2 = NULL;

  if (utf8_head)
  {
    if (fopen_s(ppf2, path, "rb+") != 0) { idealand_log("could not open file (%s) by mode rb+.", path); r = -1; goto free1;  }
    UINT8 buf[3] = { 0 }; size_t read = fread(buf, 1, 3, *ppf2);
    if (read<0) { idealand_log("cannot read from file: %s.", path); r = -1; goto free1; }
    if (read == 0) { fwrite(IdealandUtf8Head, 1, 3, *ppf2);  }
    else if(read<3 || memcmp(buf, IdealandUtf8Head,3) != 0) { idealand_log("the existing file content is not utf8: %s.", path); r = -1; goto free1; }
  }

  if (ppf == NULL) { goto free1; } else fclose(*ppf2);
  if (fopen_s(ppf2, path, mode) != 0) { idealand_log("could not open file (%s) by mode %s.", path, mode); r = -1; goto free1; } 
  return r;

free1: 
  if(*ppf2!=NULL) fclose(*ppf2);
  return r;
}


INT8* idealand_file_send_info(char *name, int no, INT64 size, int* pSendLen)
{
  if (idealand_check_file_no(no, "no", __func__) < 0) { return NULL; }
  if (idealand_check_size(size, "size", __func__) < 0) { return NULL; }
  if (idealand_check_pointer(pSendLen, "pSendLen", __func__) < 0) { return NULL; }

  INT16 nameLen = 1; if(name!=NULL) nameLen+= (INT16)(strlen(name)); *pSendLen = IdealandFiSize + nameLen;
  INT8* r = (INT8*)idealand_malloc(*pSendLen); if (r == NULL) { return r; }
  IdealandFi *pfi=(IdealandFi*)r; pfi->name_len = nameLen; pfi->no = no; pfi->size = size;
  if(name!=NULL) memcpy(r + IdealandFiSize, name, nameLen); *(r + *pSendLen - 1) = 0;//file name end
  return r;
}




FILE* idealand_file_open(char* path, const char *mode)
{
  if (idealand_check_pointer(path, "path", __func__) < 0) { return NULL; }

  FILE* pf = NULL; 
  if (fopen_s(&pf, path, mode) != 0) idealand_log("could not open file (%s) by mode %s.", path, mode);

  return pf;
}


int idealand_file_read_all(char* path, char *buf, INT64 max, int utf8Bom, int textEnd)
{
  int r = 0;
  if (idealand_check_pointer(path, (char*)"path", __func__) < 0) { return -1; }
  if (idealand_check_pointer(buf, (char*)"buf", __func__) < 0) { return -1; }
  if (idealand_check_size(max, (char*)"max", __func__) < 0) { return -1; }
  if (utf8Bom) textEnd = 1;

  FILE* pf = NULL; 
  if ((pf = idealand_file_open(path)) == NULL) { r = -1; goto free1; }
  if (utf8Bom) { if ((r = fseek(pf, 3, SEEK_SET)) != 0) { r = -1; goto free1; } }  
  if ((r = (int)fread_s(buf, max - 1, 1, max - 1, pf)) < 0) { r = -1; goto free1; }
  else if(textEnd) buf[r] = 0;

free1: if (pf != NULL) fclose(pf);
  return r;
}


