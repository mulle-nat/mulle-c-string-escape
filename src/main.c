#include "include-private.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "mulle-c-string-escape-version.h"


#define N_OUTPUT_CHARS  72    // without prefix and quotes


static void   usage( void)
{
   // eating own dogfood :)
   printf( "%s",
#include "usage.inc"
         );
   exit( 0);
}


struct emit
{
   FILE   *fp;
   //    "prefix" '"'    <characters>  '"' '\n' '\0'
   char   buf[ 3 + 1 + N_OUTPUT_CHARS + 1 + 1 + 1];
   char   *p;
   char   *sentinel;
};


static void   emit_init( struct emit *e, FILE *fp)
{
   e->fp = fp ? fp : stdout;

   memset( e->buf, ' ', 3);  // prefix

   e->buf[ 3]  = '"';
   e->p        = &e->buf[ 4];
   e->sentinel = &e->p[ N_OUTPUT_CHARS];
}


static void   emit_done( struct emit *e)
{
   fflush( e->fp);
}


// <characters> length between double quotes
static int    emit_length( struct emit *e)
{
   return( (int) (e->p - &e->buf[ 4]));
}


static void   emit_flush( struct emit *e)
{
   *e->p++ = '"';
   *e->p++ = '\n';

   fwrite( &e->buf, e->p - e->buf, 1, e->fp);

   e->p = &e->buf[ 4];
}


static void   emit_char4( struct emit *e, char c1, char c2, char c3, char c4)
{
   if( &e->p[ 4] > e->sentinel)
      emit_flush( e);

   *e->p++ = c1;
   *e->p++ = c2;
   *e->p++ = c3;
   *e->p++ = c4;
}


static void   emit_char2( struct emit *e, char c, char d)
{
   if( &e->p[ 2] > e->sentinel)
      emit_flush( e);

   *e->p++ = c;
   *e->p++ = d;
}

static void   emit_char( struct emit *e, char c)
{
   if( &e->p[ 1] > e->sentinel)
      emit_flush( e);

   *e->p++ = c;
}


static inline char   tohexnybble( int v)
{
   return( (v >= 10) ? ('a' - 10 + v) : '0' + v);
}


static inline char   tooct( int v)
{
   return( '0' + v);
}


static int  is_hex_char( int c)
{
   return( (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F'));
}

static int  is_number( int c)
{
   return( (c >= '0' && c <= '9'));
}


//
// everything a little more complicated, because I prefer hex code
// but if you have 0xfe and then a hex char follows, the compiler sees this
// as more hex code :( If this happens use octal instead.
//
static int  emit_unprintable( struct emit *e, int c, int d)
{
   if( c == 0 && ! is_number( d))
      emit_char2( e, '\\', '0');
   else
      if( ! is_hex_char( d))
         emit_char4( e, '\\', 'x', tohexnybble( c >> 4), tohexnybble( c & 0xF));
      else
         emit_char4( e, '\\', tooct( (c >> 6) & 0x7), tooct( (c >> 3) & 0x7), tooct( c & 0x7));
}


// emit 'c'
static void   emit_char_escape_if_needed( struct emit *e, int c, int d)
{
   if( c >= 127)
   {
      emit_unprintable( e, c, d);
      return;
   }

   switch( c)
   {
   // wor
   case '\r' : emit_char2( e, '\\', 'r'); break;
   case '\n' : emit_char2( e, '\\', 'n'); emit_flush( e); break;

   case '"'  : emit_char2( e, '\\', '"'); break;
   case '?'  : emit_char2( e, '\\', '?'); break;  // trigraph protect
   case '\\' : emit_char2( e, '\\', '\\'); break;
   case '\a' : emit_char2( e, '\\', 'a'); break;
   case '\b' : emit_char2( e, '\\', 'b'); break;
   case '\e' : emit_char2( e, '\\', 'e'); break;
   case '\f' : emit_char2( e, '\\', 'f'); break;
   case '\t' : emit_char2( e, '\\', 't'); break;
   case '\v' : emit_char2( e, '\\', 'v'); break;
   default   :
      if( isprint( c))
         emit_char( e, c);
      else
         emit_unprintable( e, c, d);
   }
}


int  main( int argc, char *argv[])
{
   struct emit  e;
   int          c;
   int          d;

   if( argc != 1)
      usage();

   c = getc_unlocked( stdin);
   if( c == EOF)
      return( 0);

   emit_init( &e, stdout);

   // need a look ahead (d)
   do
   {
      d = getc_unlocked( stdin);
      emit_char_escape_if_needed( &e, c, d);
      c = d;
   }
   while( c != EOF);

   if( emit_length( &e))
      emit_flush( &e);

   emit_done( &e);

   return( 0);
}
