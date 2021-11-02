# mulle-c-string-escape

#### ➿ mulle-c-string-escape turns data into C-strings

Non-ASCII characters will be escaped to hex or octal. C-escapes are used for
known C escapes like '\b'. The output is separated into lines of approximately
even length and does not exceed the chosen line length. An example output
is [usage.inc](src/usage.inc)
You can feed it binary or text files.

You can then easily `#include` the output as string contents, or copy/paste it
into your C program like so:

```
static char  text[] =
#include "data.inc"
;
```

> #### Note
>
> C will append a "\0" to strings!

See the [usage.txt](src/usage.txt) for options.

## Example

Use mulle-c-string-escape to create a string representation of itself. We use
`cmake` here instead of `mulle-sde craft --release` here too ease the fear
of the unkown:

```
mkdir -p kitchen/Release
cmake -B kitchen/Release -DCMAKE_BUILD_TYPE=Release
cmake --build kitchen/Release
./kitchen/Release/mulle-c-string-escape \
   ./kitchen/Release/mulle-c-string-escape \
   x.inc
```

To verify that this has actually worked:

```
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

```
mulle-sde craft
```

## Dedication

This little tool is dedicated to the memory of Oliver Mondry (mondry@me.com)
