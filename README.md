# mulle-c-string-escape

#### ➿ mulle-c-string-escape turns data into C-strings

It will escape non-ASCII characters to hex or octal, and use C-escapes for the
known C escapes. It will separate the output into lines not longer than
80 charactes.

You can easily `#include` the output, or just copy/paste it.

Reads from standard input and writes to standard output. You can also use
it on binary files.

So use it with redirection like:

```console
mulle-c-string-escape < mytext.txt
```

## Build

This is a [mulle-sde](https://mulle-sde.github.io/) project.

It comes with its own virtual environment and list of dependencies.
To fetch and build everything say:

```
mulle-sde craft
```
