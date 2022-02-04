/// structs and associated consts
typedef struct _finddata_t IdealandFd;



#pragma pack(1) 
/* the name_len includes the \0 ending of the name string */
typedef struct Idealand_file_info { INT16 name_len; INT64 no; INT64 size;} IdealandFi;
#pragma pack() 
int IdealandFiSize = sizeof(IdealandFi);
/* for checking the consistence of memory layout of the struct object between the 2 endpoints of a socket connection. */
IdealandFi IdealandFiCheck = { 0x1234, 0x123456789abcdef0, 0x0edcba987654321f };



typedef struct Idealand_key_value { void *pk; void* pv;} IdealandKV;
int IdealandKVSize = sizeof(IdealandKV);



/* if a struct is transfered and restored by socket, add it here. */
int IdealandStructsSize = IdealandFiSize;



typedef struct idealand_main_args { char mode; char buf_conf[IdealandConfMaxSize]; char* server; char* collection; int start; int end; int conf_ok; } IdealandMainArgs;
