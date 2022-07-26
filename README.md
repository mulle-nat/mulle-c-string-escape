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
```

> #### Note
>
> C will append a '\0' character to strings! So the actual data length 
> is `sizeof( data) - 1`.

## Options

See [usage.txt](src/usage.txt) for options.

## Example

Let's use **mulle-c-string-escape** to create a C string representation of 
itself into a file "x.inc". We use `cmake` here, instead of `mulle-sde craft --release`, to ease 
the developer's fear of the unknown:

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

## Build

This is a [mulle-sde](https://mulle-sde.github.io/) project.

It comes with its own virtual environment and list of dependencies.
To fetch and build everything say:

``` sh
mulle-sde craft
```

## Dedication

To the memory of Oliver "General" Mondry (mondry@me.com)


## Author

[Nat!](//www.mulle-kybernetik.com/weblog) for
[Mulle kybernetiK](//www.mulle-kybernetik.com).

