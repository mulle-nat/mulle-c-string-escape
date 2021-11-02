#include <stdio.h>


static char  text[] =
#include "demo2.inc"
;


int   main( void)
{
   // cut of last \0
   fwrite( text, sizeof( text) - 1, 1, stdout);
   return( 0);
}