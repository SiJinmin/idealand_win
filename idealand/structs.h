/// structs and associated consts
typedef struct _finddata_t IdealandFd;
#pragma pack(1) 
/* the name_len includes the \0 ending of the name string */
typedef struct Idealand_file_info { INT16 name_len; INT64 no; INT64 size;} IdealandFi;
#pragma pack() 
const int IdealandFiSize = sizeof(IdealandFi);
/* for checking the consistence of memory layout of the struct object between the 2 endpoints of a socket connection. */
const IdealandFi IdealandFiCheck = { 0x1234, 0x123456789abcdef0, 0x0edcba987654321f };


/* if a struct is transfered and restored by socket, add it here. */
const int IdealandStructsSize = IdealandFiSize;
