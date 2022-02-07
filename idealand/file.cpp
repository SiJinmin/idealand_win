


char* idealand_file_exe_path()
{
  char* r = NULL; errno_t err = _get_pgmptr(&r); 
  if (err != 0 || r == NULL) { r = NULL; idealand_log("could not get exe file path.\n"); } else idealand_log("exe file path = %s\n", r);
  return r;
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

  if (_access(path, 0) != 0) return -1;
  struct stat status; stat(path, &status);
  if ((file_dir == 2 || file_dir == 3) && (status.st_mode & S_IFDIR) != 0) return 0;
  if ((file_dir == 1 || file_dir == 3) && (_S_IFREG & status.st_mode) != 0) return 0;
  return -1;
}



INT64 idealand_get_file_info(char* pattern, IdealandFd* pf, int print)
{
  int r = 0;
  if ((r = idealand_check_string(pattern, 0, (char*)"pattern", __func__, IdealandMaxPathLen, 1)) < 0) { return r; }
  if ((r = idealand_check_pointer(pf, (char*)"pf", __func__)) < 0) { return r; }

  intptr_t fHandle; 
  if ((fHandle = _findfirst(pattern, pf)) == -1) { if (print) idealand_log("cannot find file: %s\n", pattern); return -1; }
  _findclose(fHandle); 
  // if(name!=NULL) idealand_string_wchar_to_utf8(pf->name, name, IdealandMaxNameLen - 1);
  if (print) idealand_log("found file %s\n", pf->name);

  //do {
  //  printf("找到文件:%s,文件大小：%d\n", fileinfo.name, fileinfo.size);
  //} while (_findnext(fHandle, &fileinfo) == 0);

  return pf->size;
}
INT64 idealand_get_file_info(char* collection, int no, IdealandFd* pf, int print)
{
  INT64 r = 0;
  if ((r = idealand_check_filename(collection, (char*)"collection", __func__)) < 0) { return r; }
  if ((r = idealand_check_int(no, (char*)"no", __func__, IdealandDirMaxFilesCount, 0)) < 0) { return r; }
  if ((r = idealand_check_pointer(pf, (char*)"pf", __func__)) < 0) { return r; }

  char* search = idealand_string(IdealandMaxPathLen, NULL, (char*)"%s/%04d*", collection, no); if (search == NULL) { return -1; }
  r = idealand_get_file_info(search, pf, print); free(search); return r;
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
  else if (_mkdir(path) != 0) { idealand_log("create dir %s fail.", path); r = -1;  goto free2; }
  else { idealand_log("create dir %s succeed.\n", path); goto free1; }

free2: free(path);
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


