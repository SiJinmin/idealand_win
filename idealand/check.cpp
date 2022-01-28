

int idealand_check_string(char* str, int allowNull, char* name, char* functionName, INT64 maxLen, INT64 minLen)
{
  if (str == NULL)
  {
    if (!allowNull) { idealand_error((char*)"%s cannot be NULL in %s", name, functionName); return -1; }
    else return 0;
  }
  else
  {
    INT64 len = strlen(str);
    if (len > maxLen) { idealand_error((char*)"%s(len=%lld) cannot be longer than %d in %s", name, len, maxLen, functionName); return -1; }
    if (len < minLen) { idealand_error((char*)"%s(len=%lld) cannot be shorter than %d in %s", name, len, maxLen, functionName); return -1; }
  }
  return 0;
}



int idealand_check_filename(char* filename, char* name, char* functionName)
{
  INT64 r = 0;

  if ((r = idealand_check_string(filename, 0, name, functionName, IdealandMaxNameLen, 1)) < 0) { return (int)r; }

  char** ppChars = NULL; r = idealand_string_utf8(filename, NULL, NULL, &ppChars, IdealandMaxNameLen);
  if (r == 0) { idealand_error((char*)"%s cannot be empty in %s", name, functionName); r = -1;  } if(r<0) goto free1;
  if (r == 1 && *ppChars[0] == '.') { idealand_error("%s cannot be . in %s", name, functionName); r = -1; goto free1; }
  if (r >= 2 && *ppChars[0] == '.' && *ppChars[1] == '.') { idealand_error("%s cannot starts with .. in %s", name, functionName); r = -1; goto free1; }

  for (int i = 0; i < r; i++)
  {
    char c = *ppChars[i]; for (int j = 0; j < IdealandNameExcludeCharsCount; j++)
    {
      if (c == IdealandNameExcludeChars[j]) { idealand_error("%s cannot contain %c in %s", name, c, functionName); r = -1; break; }
    }
  }  

free1: if (ppChars != NULL) free(ppChars);
  return (int)r;
}


