/* caller should provide configuration text, and IdealandKV r[maxKVs]; 
   return the count of kv pairs found in text, or negative value for errors;  */
int idealand_text_conf(char* text, IdealandKV* r, int maxKVs);
/* return NULL if not found or error. */
char* idealand_text_conf_value(char* key, IdealandKV* r, int maxKVs);
inline char* idealand_text_conf_value(const char* key, IdealandKV* r, int maxKVs)
{  return idealand_text_conf_value((char*)key, r, maxKVs); }