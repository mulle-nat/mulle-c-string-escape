#include <stdio.h>
#include <stdlib.h>


int  main( int argc, char *argv[])
{
   int  i;
   int  c;

   for( i = 1; i < argc; i++)
   {
      c = atoi( argv[ i]);
      putchar( c);
   }
   return( 0);
}

