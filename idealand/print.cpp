


void idealand_print_clear(int preCharCount)
{
  if (idealand_check_int(preCharCount, "preCharCount", __func__, 1000, 0) < 0) { return; }
  while (preCharCount-- > 0) printf("\b \b");
}



int idealand_print_transfer_speed(INT64 usedSeconds, INT64 transferedSize, INT64 clientSize, INT64 fileSize, int preCharCount)
{
  int r = 0;
  if ((r = idealand_check_seconds(usedSeconds, "usedSeconds", __func__)) < 0) return r;
  if ((r = idealand_check_size(transferedSize, "transferedSize", __func__)) < 0) return r;
  if ((r = idealand_check_size(clientSize, "clientSize", __func__)) < 0) return r;
  if ((r = idealand_check_size(fileSize, "fileSize", __func__, 1)) < 0) return r;
  if ((r=idealand_check_int(preCharCount, "preCharCount", __func__, 1000, 0)) < 0) return r;

  double percentage = 100.0 * (transferedSize + clientSize) / fileSize;
  INT64 remainSeconds = 0, speed = 0;
  if (usedSeconds > 0) speed = (transferedSize / usedSeconds); if (speed > 0) remainSeconds = (fileSize - clientSize - transferedSize) / speed;
  char* totalText = idealand_string_size(transferedSize, 1); if(totalText == NULL) r= -1;
  char* usedTime = idealand_string_time_length(usedSeconds); if (usedTime == NULL) r= -1;
  char* remainTime = idealand_string_time_length(remainSeconds); if (remainTime == NULL) r= -1;
  char* speedText = idealand_string_size(speed); if (speedText == NULL) r= -1;
  if (r >= 0) 
  {
    idealand_print_clear(preCharCount);
    r = printf("%s(%.1f%%), %s/%s, %s/sec", totalText, percentage, usedTime, remainTime, speedText);
  }
  if (totalText != NULL)free(totalText); if (usedTime != NULL)free(usedTime); if (remainTime != NULL)free(remainTime); if (speedText != NULL)free(speedText);
  return r;
}

void idealand_print_encoding(char * str)
{
  if (str == NULL) return;

  UINT8 c; UINT8* p = (UINT8*)str; while (c = *p++)
  {
    printf("%hx(%u) ", c, c);
  }
  printf("\n");
}

