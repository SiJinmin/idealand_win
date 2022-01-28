/* clear preCharCount chars of the current line of stdout */
void idealand_print_clear(int preCharCount);
/* returns the printed char count, or negative value if error occur;
   clear preCharCount chars before the printing; */
int idealand_print_transfer_speed(INT64 usedSeconds, INT64 transferedSize, INT64 clientSize, INT64 fileSize, int preCharCount);

void idealand_print_encoding(char* str);
inline void idealand_print_encoding(const char* str){ idealand_print_encoding((char*)str); }
