#include <stdio.h>
#include <stdlib.h>
#include "../idealand/dizangmulun/main.cpp"


int main()
{
  dizangmulu3();
  getchar();
  return 0;

  char* r = NULL; _get_pgmptr(&r); printf("exe file path = %s", r); 
   return 0;

  long count = 0;
  while (getchar() != '\n') count++;
  printf("%ld characters.\n", count);
  return 0;

  printf("Hello World!\n"); 
  char c = getchar();  int isEOF = c == EOF; int isNotEOF = c != EOF;
  printf("EOF=%d, isEOF=%d, isNotEOF=%d\n", EOF, isEOF, isNotEOF );

}


