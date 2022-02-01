int idealand_text_conf(char* text, IdealandKV* pKvs, int maxKVs)
{
  int r = 0;
  if ((r = idealand_check_pointer(text, (char*)"text", __func__)) < 0) return r;
  if ((r = idealand_check_pointer(pKvs, (char*)"pKvs", __func__)) < 0) return r;
  if ((r = idealand_check_size(maxKVs, (char*)"maxKVs", __func__)) < 0) return r;

  char* p = text, c; int lineStart = 1, commentLine = 0, gotKey=0, remain= maxKVs;
  while (remain>0 && (c = *p++))
  {
    if (lineStart)
    {
      if (c == ' ' || c == '\r' || c == '\n') continue;
      else if (c == '#') { lineStart = 0; commentLine = 1; }
      else { lineStart = 0; pKvs[maxKVs - remain].pk = p-1; }
    }
    else
    {
      if (c == '=') { *(p - 1) = '\0'; gotKey = 1;  pKvs[maxKVs - remain].pv = p;   }
      else if (c == '\n') 
      {
        if (commentLine) { lineStart = 1, commentLine = 0,  gotKey = 0; }
        else 
        { 
          if(!gotKey){ lineStart = 1, commentLine = 0, gotKey = 0; }
          else{ *(p - 1) = '\0'; lineStart = 1, commentLine = 0,  gotKey = 0, remain--;  }          
        }
        if (*(p - 2) == '\r') { *(p - 2) = '\0'; }
        if (*p == '\r') { *p = '\0'; }
      }
    }
  }
  return maxKVs - remain;
}



char* idealand_text_conf_value(char* key, IdealandKV* pKvs, int maxKVs)
{
  int r = 0;
  if ((r = idealand_check_pointer(key, (char*)"key", __func__)) < 0) return NULL;
  if ((r = idealand_check_pointer(pKvs, (char*)"pKvs", __func__)) < 0) return NULL;
  if ((r = idealand_check_size(maxKVs, (char*)"maxKVs", __func__)) < 0) return NULL;

  for (int i = 0; i < maxKVs; i++)
  {
    IdealandKV kv = pKvs[i]; if (strcmp(key, (const char *)kv.pk) == 0) return (char *)kv.pv;
  }
  return NULL;
}