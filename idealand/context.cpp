


void idealand_error(const char* format, ...)
{
  printf("\nidealand error: "); va_list args; va_start(args, format); vprintf(format, args); va_end(args); printf("\n");
}



int idealand_check_set_runtime()
{
  printf("\nidealand_check_runtime() ...\n");

  // 检查大小端
  UINT16 a = 0x1234; UINT8* p1 = (UINT8*)&a; UINT8* p2 = p1 + 1;
  if (*p1 != 0x34 || *p2 != 0x12) { idealand_error("the runtime is not little endian."); return -1; }

  // 创建文件夹
  char* dir = idealand_file_exe_dir(); if (dir == NULL) return -1;
  IdealandDataPath = idealand_file_mkdir(IdealandDataDirName, dir); if (IdealandDataPath == NULL ) return -1;
  printf("data dir = %s\n", IdealandDataPath);
  if (idealand_file_change_work_dir(IdealandDataPath) < 0) return -1;

  printf("check runtime succeed.\n\n"); return 0;
}


void* idealand_malloc(INT64 size)
{
  if (idealand_check_malloc_size(size, "size", __func__) < 0) { return NULL; }
  void* r = malloc(size); if (r == NULL) { idealand_error("malloc %lld bytes fail.", size); } return r;
}