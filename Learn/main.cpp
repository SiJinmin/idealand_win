
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>

// #include "../idealand/dizangmulun/main.cpp"


int main(int argc, char** argv)
{

  int r = 0;

  char* server = argv[1];

  printf("%s\n", server);

  uint8_t * p = (uint8_t*)server;
  uint8_t c ;
  while (c=*p++)
  {
    printf("%hx ", c);
  }
  p = (uint8_t*)server;
  while (c = *p++)
  {
    printf("%d ", c);
  }
  printf("\n");
  return 0;



  /*dizangmulu3();
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
  printf("EOF=%d, isEOF=%d, isNotEOF=%d\n", EOF, isEOF, isNotEOF );*/

}


