# mulle-c-string-escape

#### ➿ mulle-c-string-escape turns data into C strings

Non-ASCII characters will be escaped to hex or octal. C escape sequences are used 
for the predefined sequences like '\b'. The output is separated into lines of approximately
even length and does not exceed the chosen line length. An example of a 
conversion is [usage.txt](src/usage.txt) being turned into [usage.inc](src/usage.inc).
You can feed it binary or text files.

You can copy/paste the output as string contents into your C source or `#include` 
the output like so:

``` c
static char  data[] =
#include "data.inc"
;
#define s_data ( sizeof( data) - 1)
```

> #### Note
>
> C will append a '\0' character to strings! So the actual data length 
> is `sizeof( data) - 1`.





## Usage

```
mulle-c-string-escape error:unknown input --help

Usage:
   mulle-c-string-escape [options] [infile] [outfile]

   ➿ mulle-c-string-escape turns data into C-strings

   Non-ASCII characters will be escaped to hex or octal. C-escapes are used for
   known C escapes like '\b'. The output is separated into lines of approximately
   even length and does not exceed the chosen line length.
   You can feed it binary or text files.

   You can then easily `#include` the output, or copy/paste it into your
   C program like so:

      static char  data[] =
      #include "data.inc"
      ;
      #define s_data  (sizeof( data) - 1)

Example:
   mulle-c-string-escape -0 -l 120 < mytext.txt

Options:
   -[01234] : zero to four space prefix (TAB)
   -c       : append ".inc" to infile to generate outfile
   -e       : escape TAB characters
   -h       : this help
   -l <n>   : targetted line length (80)
   -n       : don't create a new line for linefeeds in text
   -p <s>   : use a custom prefix instead of spaces or TAB
   -q       : don't print helpful C code
   -t <s>   : visual size of TAB in text (8)
   -v       : print version information and exit

Dedication:
   To the memory of Oliver "General" Mondry (mondry@me.com)


```


## Example

Let's use **mulle-c-string-escape** to create a C string representation of 
itself into a file "x.inc". We use `cmake` here, instead of
`mulle-sde craft --release`, to ease the developer's fear of the unknown:

``` sh
mkdir -p kitchen/Release
cmake -B kitchen/Release -DCMAKE_BUILD_TYPE=Release
cmake --build kitchen/Release --config Release
./kitchen/Release/mulle-c-string-escape \
   ./kitchen/Release/mulle-c-string-escape \
   x.inc
```

To verify that this has actually worked:

``` sh
cat <<EOF > x.c
#include <stdio.h>


static char  text[] = "" // useful in case x.inc can be empty
#include "x.inc"
;

int  main( int argc, char *argv[])
{
   FILE   *fp;

   fp = fopen( "verify.exe", "wb");
   fwrite( text, sizeof( text) - 1, 1, fp); // -1 because C appends a \0
   return( fclose( fp));
}
EOF

cc -o x x.c
./x
diff verify.exe ./kitchen/Release/mulle-c-string-escape
```






## Add

Use [mulle-sde](//github.com/mulle-sde) to add mulle-c-string-escape to your project:

``` sh
mulle-sde add github:mulle-c/mulle-c-string-escape
```

To only add the sources of mulle-c-string-escape with dependency
sources use [clib](https://github.com/clibs/clib):


``` sh
clib install --out src/mulle-c mulle-c/mulle-c-string-escape
```

Add `-isystem src/mulle-c` to your `CFLAGS` and compile all the sources that were downloaded with your project.


## Install

### Install with mulle-sde

Use [mulle-sde](//github.com/mulle-sde) to build and install mulle-c-string-escape:

``` sh
mulle-sde install --prefix /usr/local \
   https://github.com/mulle-c/mulle-c-string-escape/archive/latest.tar.gz
```

### Manual Installation


Install **mulle-c-string-escape** into `/usr/local` with [cmake](https://cmake.org):

``` sh
cmake -B build \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DCMAKE_PREFIX_PATH=/usr/local \
      -DCMAKE_BUILD_TYPE=Release &&
cmake --build build --config Release &&
cmake --install build --config Release
```


## Author

[Nat!](https://mulle-kybernetik.com/weblog) for Mulle kybernetiK


## Dedication

To the memory of Oliver "General" Mondry (mondry@me.com)



