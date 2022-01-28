


char* idealand_file_exe_path()
{
  char* r = NULL; errno_t err = _get_pgmptr(&r); 
  if (err != 0 || r == NULL) { r = NULL; idealand_error("could not get exe file path."); } else printf("exe file path = %s\n", r);
  return r;
}
char* idealand_file_exe_dir()
{
  char* r = idealand_file_exe_path();  if (r == NULL) return NULL;
  INT64 rindex = idealand_string_rindex('\\', r); if (rindex < 0) rindex = idealand_string_rindex('/', r);
  if (rindex < 0) { idealand_error("could not find dir sep in exe path in %s.", (char*)__func__); return NULL; }
  r[rindex+1] = 0; printf("exe dir = %s\n", r); return r;
}



int idealand_file_exist(char* path, int file_dir)
{
  int r = 0;
  if ((r = idealand_check_path(path, (char*)"path", (char*)__func__)) < 0) { return r; }
  if ((r = idealand_check_int(file_dir, (char*)"file_dir", (char*)__func__, 3, 1)) < 0) { return r; }

  if (_access(path, 0) != 0) return -1;
  struct stat status; stat(path, &status);
  if ((file_dir == 2 || file_dir == 3) && (status.st_mode & S_IFDIR) != 0) return 0;
  if ((file_dir == 1 || file_dir == 3) && (_S_IFREG & status.st_mode) != 0) return 0;
  return -1;
}



INT64 idealand_file_info(char* pattern, IdealandFd* pf, int print, char* name)
{
  int r = 0;
  if ((r = idealand_check_string(pattern, 0, (char*)"pattern", (char*)__func__, IdealandMaxPathLen, 1)) < 0) { return r; }
  if ((r = idealand_check_pointer(pf, (char*)"pf", (char*)__func__)) < 0) { return r; }

  intptr_t fHandle; // wchar_t wpattern[IdealandMaxNameLen]; idealand_string_utf8_to_wchar(pattern, wpattern, IdealandMaxNameLen - 1);
  if ((fHandle = _findfirst(pattern, pf)) == -1) { if (print) printf("cannot find file: %s\n", pattern); return -1; } 
  _findclose(fHandle); 
  // if(name!=NULL) idealand_string_wchar_to_utf8(pf->name, name, IdealandMaxNameLen - 1);
  if (print) printf("found file %s\n", pf->name);

  //do {
  //  printf("找到文件:%s,文件大小：%d\n", fileinfo.name, fileinfo.size);
  //} while (_findnext(fHandle, &fileinfo) == 0);

  return pf->size;
}
INT64 idealand_file_info(char* collection, int no, IdealandFd* pf, int print, char *name)
{
  INT64 r = 0;
  if ((r = idealand_check_filename(collection, (char*)"collection", (char*)__func__)) < 0) { return r; }
  if ((r = idealand_check_int(no, (char*)"no", (char*)__func__, IdealandDirMaxFilesCount, 0)) < 0) { return r; }
  if ((r = idealand_check_pointer(pf, (char*)"pf", (char*)__func__)) < 0) { return r; }

  char* search = idealand_string(IdealandMaxPathLen, NULL, (char*)"%s/%04d*", collection, no); if (search == NULL) { return -1; }
  r = idealand_file_info(search, pf, print, name); free(search); return r;
}



int idealand_file_change_work_dir(char* path)
{
  int r = 0;
  if ((r = idealand_file_exist(path,2)) < 0) return r;

  if (_chdir(path) != 0) { idealand_error("change work dir to %s fail.", path); return -1; }
  printf("change work dir to %s succeed.\n", path); return 0;
}




char * idealand_file_mkdir(char* name, char* parent)
{
  int r = 0; char* path=NULL;

  if ((r = idealand_check_filename(name, (char*)"name", (char*)__func__)) < 0) { return NULL; }
  char* parent2=NULL; if (parent != NULL && (parent2 = idealand_string_normalize_path(parent, 1)) == NULL) return NULL;
  if (parent2!=NULL && (r = idealand_file_exist(parent2, 2)) < 0) { printf("parent dir(%s) do not exists, cannot creat dir in it.\n", parent2); r = -1; goto free1; }

  if(parent2 != NULL) path = idealand_string(2048, NULL, (char*)"%s%s%c", parent2, name, IdealandPathSep);
  else path = idealand_string(2048, NULL, (char*)"%s%c", name, IdealandPathSep);
  
  if (path == NULL) { r = -1; goto free1; }
  if (idealand_file_exist(path, 2) >= 0) {  /* printf("dir(%s) exists, no need to create.\n", path); */  goto free1; }
  else if (_mkdir(path) != 0) { idealand_error("create dir %s fail.", path); r = -1;  goto free2; }
  else { printf("create dir %s succeed.\n", path); goto free1; }

free2: free(path);
free1:  if (parent2 != parent && parent2 != NULL) free(parent2);
  if (r >= 0) return path; else return NULL;
}



INT8* idealand_file_send_info(char *name, int no, INT64 size, int* pSendLen)
{
  if (idealand_check_file_no(no, (char*)"no", (char*)__func__) < 0) { return NULL; }
  if (idealand_check_size(size, (char*)"size", (char*)__func__) < 0) { return NULL; }
  if (idealand_check_pointer(pSendLen, (char*)"pSendLen", (char*)__func__) < 0) { return NULL; }

  INT16 nameLen = 1; if(name!=NULL) nameLen+= (INT16)(strlen(name)); *pSendLen = IdealandFiSize + nameLen;
  INT8* r = (INT8*)idealand_malloc(*pSendLen); if (r == NULL) { return r; }
  IdealandFi *pfi=(IdealandFi*)r; pfi->name_len = nameLen; pfi->no = no; pfi->size = size;
  if(name!=NULL) memcpy(r + IdealandFiSize, name, nameLen); *(r + *pSendLen - 1) = 0;//file name end
  return r;
}




FILE* idealand_file_open(char* path, char *mode)
{
  if (idealand_check_pointer(path, (char*)"path", (char*)__func__) < 0) { return NULL; }

  FILE* pf = NULL; 
  if (fopen_s(&pf, path, mode) != 0) idealand_error("could not open file (%s) for binary read.", path);
  return pf;
}

int idealand_file_read_all(char* path, char *buf, INT64 max, int utf8Bom, int textEnd)
{
  int r = 0;
  if (idealand_check_pointer(path, (char*)"path", (char*)__func__) < 0) { return -1; }
  if (idealand_check_pointer(buf, (char*)"buf", (char*)__func__) < 0) { return -1; }
  if (idealand_check_size(max, (char*)"max", (char*)__func__) < 0) { return -1; }
  if (utf8Bom) textEnd = 1;

  FILE* pf = NULL; 
  if ((pf = idealand_file_open(path)) == NULL) { r = -1; goto free1; }
  if (utf8Bom) { if ((r = fseek(pf, 3, SEEK_SET)) != 0) { r = -1; goto free1; } }  
  if ((r = (int)fread_s(buf, max - 1, 1, max - 1, pf)) < 0) { r = -1; goto free1; }
  else if(textEnd) buf[r] = 0;

free1: if (pf != NULL) fclose(pf);
  return r;
}


