


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
  if (rindex < 0) { idealand_error("could not find dir sep in exe path in %s.", __func__); return NULL; } 
  r[rindex+1] = 0; printf("exe dir = %s\n", r); return r;
}



int idealand_file_exist(const char* path, int file_dir)
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



INT64 idealand_file_info(const char* pattern, IdealandFd* pf, int print)
{
  int r = 0;
  if ((r = idealand_check_string(pattern, 0, "pattern", __func__, IdealandMaxPathLen, 1)) < 0) { return r; }
  if ((r = idealand_check_pointer(pf, "pf", __func__)) < 0) { return r; }

  intptr_t fHandle;
  if ((fHandle = _findfirst(pattern, pf)) == -1) { if (print) printf("cannot find file: %s\n", pattern); return -1; }
  _findclose(fHandle); if (print) printf("found file %s for %s\n", pf->name, pattern);

  //do {
  //  printf("找到文件:%s,文件大小：%d\n", fileinfo.name, fileinfo.size);
  //} while (_findnext(fHandle, &fileinfo) == 0);

  return pf->size;
}
INT64 idealand_file_info(const char* collection, int no, IdealandFd* pf, int print)
{
  INT64 r = 0;
  if ((r = idealand_check_filename(collection, "collection", __func__)) < 0) { return r; }
  if ((r = idealand_check_int(no, "no", __func__, IdealandDirMaxFilesCount, 0)) < 0) { return r; }
  if ((r = idealand_check_pointer(pf, "pf", __func__)) < 0) { return r; }

  char* search = idealand_string(IdealandMaxPathLen, NULL, "%s/%04d*", collection, no); if (search == NULL) { return -1; }
  r = idealand_file_info(search, pf, print); free(search); return r;
}



int idealand_file_change_work_dir(const char* path)
{
  int r = 0;
  if ((r = idealand_file_exist(path,2)) < 0) return r;

  if (_chdir(path) != 0) { idealand_error("change work dir to %s fail.", path); return -1; }
  printf("change work dir to %s succeed.\n", path); return 0;
}




char * idealand_file_mkdir(const char* name, char* parent)
{
  int r = 0; char* path=NULL;

  if ((r = idealand_check_filename(name, "name", __func__)) < 0) { return NULL; }
  char* parent2=NULL; if (parent != NULL && (parent2 = idealand_string_normalize_path(parent, 1)) == NULL) return NULL;
  if (parent2!=NULL && (r = idealand_file_exist(parent2, 2)) < 0) { printf("parent dir(%s) do not exists, cannot creat dir in it.\n", parent2); r = -1; goto free1; }

  if(parent2 != NULL) path = idealand_string(2048, NULL, "%s%s%c", parent2, name, IdealandPathSep); 
  else path = idealand_string(2048, NULL, "%s%c", name, IdealandPathSep);
  
  if (path == NULL) { r = -1; goto free1; }
  if (idealand_file_exist(path, 2) >= 0) {  /* printf("dir(%s) exists, no need to create.\n", path); */  goto free1; }
  else if (_mkdir(path) != 0) { idealand_error("create dir %s fail.", path); r = -1;  goto free2; }
  else { printf("create dir %s succeed.\n", path); goto free1; }

free2: free(path);
free1:  if (parent2 != parent && parent2 != NULL) free(parent2);
  if (r >= 0) return path; else return NULL;
}



INT8* idealand_file_send_info(const char *name, int no, INT64 size, int* pSendLen)
{
  if (idealand_check_file_no(no, "no", __func__) < 0) { return NULL; }
  if (idealand_check_size(size, "size", __func__) < 0) { return NULL; }
  if (idealand_check_pointer(pSendLen, "pSendLen", __func__) < 0) { return NULL; }

  INT16 nameLen = 1; if(name!=NULL) nameLen+= (INT16)strlen(name); *pSendLen = IdealandFiSize + nameLen;
  INT8* r = (INT8*)idealand_malloc(*pSendLen); if (r == NULL) { return r; }
  IdealandFi *pfi=(IdealandFi*)r; pfi->name_len = nameLen; pfi->no = no; pfi->size = size;
  if(name!=NULL) memcpy(r + IdealandFiSize, name, nameLen); *(r + *pSendLen - 1) = 0;//file name end
  return r;
}


