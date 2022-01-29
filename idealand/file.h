/* return the exe file path, NULL if error occur. */
char* idealand_file_exe_path();
/* return the exe file dir path, NULL if error occur. */
char* idealand_file_exe_dir();



/* file_dir is 1 for file, 2 for dir, 3 for file or dir. */
int idealand_file_exist(char* path, int file_dir);



/* return file size in bytes; -1 for not existing file;
   other negative values if other errors occur; */
INT64 idealand_get_file_info(char* path, IdealandFd* pf, int print = 0);
inline INT64 idealand_get_file_info(const char* path, IdealandFd* pf, int print = 0)
{ return idealand_get_file_info((char*)path, pf, print); }
/* return file size in bytes; -1 for not existing file;
   other negative values if other errors occur; */
INT64 idealand_get_file_info(char* dir, int no, IdealandFd* pf, int print = 0);



int idealand_file_change_work_dir(char * path);



char* idealand_file_create(char* name, char* parent);
/* caller should free(r) if not NULL*/
char* idealand_file_mkdir(char* name, char* parent=NULL);



/* return send data buffer pointer(r) and length(pSendLen);
   caller should free(r) if not NULL;
   send data buffer contains IdealandFi and the name string;
   name can be NULL; */
INT8* idealand_file_send_info(char* name, int no, INT64 size, int* pSendLen);



/* open file for binary read, return NULL for error. 
   caller should fclose(r) if not NULL */
FILE* idealand_file_open(char* path, char *mode= (char*)"rb");

/* caller should provide char buf[max]
   returns the read char count, or negative value for errors. */
int idealand_file_read_all(char* path, char* buf, INT64 max, int utf8Bom=0, int textEnd=0);

