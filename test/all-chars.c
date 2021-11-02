#include <stdio.h>
#include <stdlib.h>


int  main( int argc, char *argv[])
{
   int  i;
   int  start;
   int  end;

   start=atoi( argv[ 1]);
   end=atoi( argv[ 2]);
   for( i = start; i <= end; i++)
      putchar( i);
   return( 0);
}

