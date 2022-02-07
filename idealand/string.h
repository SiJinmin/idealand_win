/* caller should free(r) if not NULL; 
   pcount return the char count if pcount is not NULL; 
   the max strlen(r) is max-1 as the last char must be 0;  */
char* idealand_string(int max, int* pcount, const char* format, ...);
void idealand_string_replace(char from, char to, char* str);
/* caller should free(r) if r!=path and not NULL. */
char* idealand_string_normalize_path(char* path, int dirEnd);
/* return the count of removed chars, or negative values for error. */
INT64 idealand_string_remove_repeat(char repeat, char* str);
/* caller should free(r) if not NULL; 
   pcount return the char count if pcount is not NULL;
   full controls the parts included, 1 means all parts, 0 means only the highest part;   */
char* idealand_string_time_length(INT64 seconds, int full = 0, int* pcount=NULL);
/* caller should free(r) if not NULL; 
   pcount return the char count if pcount is not NULL;  
   full controls the parts included, 1 means all parts, 0 means only the highest part; */
char* idealand_string_size(INT64 bytes, int full = 0, int* pcount = NULL);
/* return the index of the first c in str, negative values if error. */
INT64 idealand_string_index(char c, char *str);
/* return the index of the last c in str, negative values if error. */
INT64 idealand_string_rindex(char c, char* str);
/* return total char count by utf8 encoding, negative values if error.
   caller should free(*pppChars) if maxChars is greater than 0 and not NULL*/
INT64 idealand_string_utf8(char* str, INT64* pAsciiCount=NULL, INT64* pOtherCount=NULL, char*** pppChars =NULL, int maxChars=0);
