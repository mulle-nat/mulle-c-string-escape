#include "include-private.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "mulle-c-string-escape-version.h"


static void   usage( void)
{
   // eating own dogfood :)
   printf( "%s", "" // keep ""
#include "usage.inc"
         );
   exit( 0);
}

static inline size_t _calc_emit_buf_size( size_t prefix_length, 
                                          size_t text_length)
{
   //    "prefix" '"'    <characters>  '"' '\n' '\0'
   return( prefix_length + 1 + text_length + 1 + 1 + 1);
}

struct emit
{
   FILE   *fp;
   char   *buf;
   char   *p;
   char   *sentinel;
   char   *start;
   char   *end;

   int    flush_on_lf;
   int    escape_tab;
   int    tab_size;
};


static void   emit_init( struct emit *e, 
                         char *prefix, 
                         unsigned int line_length, 
                         unsigned int tab_size,
                         FILE *fp)
{
   size_t   prefix_length;
   size_t   text_length;

   memset( e, 0, sizeof( *e));

   if( ! line_length)
      line_length = 80;

   if( line_length <= 3)
      usage();

   text_length = line_length - 3;
   
   // todo: should parse prefix for tab charactes and adjust length
   if( ! prefix)
   {
      if( text_length <= e->tab_size)
         usage();
      text_length  -= e->tab_size; // 8 i 
      prefix        = "\t";
      prefix_length = 1;
   }
   else
   {
      prefix_length = strlen( prefix);
      if( text_length <= prefix_length)
         usage();
      text_length  -= prefix_length;
   }

   e->buf = malloc( _calc_emit_buf_size( prefix_length, text_length));
   if( ! e->buf)
      abort();

   memcpy( e->buf, prefix, prefix_length);  // prefix
   e->buf[ prefix_length] = '"';
   e->start               = &e->buf[ prefix_length + 1];
   e->p                   = e->start;
   e->end                 = &e->p[ text_length];
   e->sentinel            = e->end;

   e->escape_tab          = 0;
   e->flush_on_lf         = 1;
   e->tab_size            = tab_size;

   e->fp                  = fp ? fp : stdout;
}


static inline void   emit_set_escape_tab( struct emit *e, int escape_tab)
{
   e->escape_tab = escape_tab;
}


static inline void   emit_set_flush_on_lf( struct emit *e, int flush_on_lf)
{
   e->flush_on_lf = flush_on_lf;
}


static void   emit_done( struct emit *e)
{
   if( e)
   {
      free( e->buf);
      fflush( e->fp);
      if( e->fp != stdout)
         fclose( e->fp);
   }
}


// <characters> length between double quotes
static int    emit_length( struct emit *e)
{
   return( (int) (e->p - e->start));
}


static void   emit_flush( struct emit *e)
{
   *e->p++ = '"';
   *e->p++ = '\n';

   fwrite( e->buf, (int) (e->p - e->buf), 1, e->fp);

   e->p        = e->start;
   e->sentinel = e->end;
}


static void   emit_tab( struct emit *e)
{
   if( &e->p[ e->tab_size] > e->sentinel)
      emit_flush( e);

   *e->p++ = '\t';
   // reduce sentinel as visible space is less now
   e->sentinel = &e->sentinel[ - e->tab_size];
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
static void  emit_unprintable( struct emit *e, int c, int d)
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
   case '\n' : emit_char2( e, '\\', 'n'); if( e->flush_on_lf) emit_flush( e); break;

   case '"'  : emit_char2( e, '\\', '"'); break;
   case '?'  : emit_char2( e, '\\', '?'); break;  // trigraph protect
   case '\\' : emit_char2( e, '\\', '\\'); break;
   case '\a' : emit_char2( e, '\\', 'a'); break;
   case '\b' : emit_char2( e, '\\', 'b'); break;
   case '\e' : emit_char2( e, '\\', 'e'); break;
   case '\f' : emit_char2( e, '\\', 'f'); break;
   case '\t' : e->escape_tab ? emit_char2( e, '\\', 't') : emit_tab( e); break;
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
   struct emit    e;
   int            c;
   int            d;
   int            i;
   int            escape_tab;
   FILE           *fin;
   FILE           *fout;
   int            flush_on_lf;
   char           *prefix;
   unsigned int   line_length;
   unsigned int   tab_size;

   line_length = 0;  // default: 80
   prefix      = NULL;
   escape_tab  = 0;
   flush_on_lf = 1;
   tab_size    = 8;

   for( i = 1; i < argc; i++)
   {
      if( argv[ i][ 0] == '-')
         switch( argv[ i][ 1])
         {
         case '0' :
            prefix = "";
            continue;
         case '1' :
            prefix = " ";
            continue;
         case '2' :
            prefix = "  ";
            continue;
         case '3' :
            prefix = "   ";
            continue;
         case '4' :
            prefix = "    ";
            continue;

         case 'e' :
            escape_tab = 1;
            continue;

         case 'l' :
            if( ++i >= argc)
               usage();
            line_length = (unsigned int) atoi( argv[ i]);
            if( line_length < 4)
               usage();
            continue;

         case 'n' :
            flush_on_lf = 0;
            continue;

         case 'p' :
            if( ++i >= argc)
               usage();
            prefix = argv[ i];
            continue;

         case 't' :
            if( ++i >= argc)
               usage();
            tab_size = atoi( argv[ i]);
            if( (int) tab_size <= 0)
               usage();
            continue;

         default:
            usage();
         }
      break;
   }

   fin = stdin;
   if( i < argc)
   {
      fin = fopen( argv[ i], "rb");
      if( ! fin)
      {
         perror( "infile");
         exit( 1);
      }   
      ++i;
   }

   fout = stdout;
   if( i < argc)
   {
      fout = fopen( argv[ i], "wb");
      if( ! fin)
      {
         perror( "outfile");
         exit( 1);
      }   
      ++i;
   }

   if( i != argc)
      usage();

   c = getc_unlocked( fin);
   if( c == EOF)
      return( 0);

   emit_init( &e, prefix, line_length, tab_size, fout);
   emit_set_escape_tab( &e, escape_tab);
   emit_set_flush_on_lf( &e, flush_on_lf);

   // need a look ahead (d)
   do
   {
      d = getc_unlocked( fin);
      emit_char_escape_if_needed( &e, c, d);
      c = d;
   }
   while( c != EOF);

   if( emit_length( &e))
      emit_flush( &e);

   emit_done( &e);

   return( 0);
}
