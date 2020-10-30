# gc - Grammar Correcter

This started out of an idea spawned by a question on an exam paper that
asked me to write an algorithm to automatically capitalise letters at the
beginning of sentences. I took the algorithm I came up with a bit further,
transformed the pseudocode into C, and this was the result.

## Features

* Automatically capitalises the first letter of sentences (only if they start with a letter)
* Collapses any excessive white-space to a single space character
* Completely trims off white-space at the start of the text
* Reads from files given as command-line args (or stdin), writes to standard output

## Compiling

All you need to do is run `make` for a debug build, or `make release` for
a release build. You can also run `make clean` to clean up generated object
files and the binary itself.

## Usage

Just run `gc <filename>`, where filename is the name of a file you'd like to fix. For example:

**test.txt:**

    this is   a test  file.        it's not very well      written.
    that's the point.
    
    here's another        paragraph.

```sh
./gc test.txt
```

For complete help and usage examples, run `./gc --help`:

    Usage:
            ./gc # Read from STDIn
            ./gc file1|-[ file2|-[ ...]]

    ./gc - Grammar Correcter  Copyright (C) 2020  Michael Connor Buchan
    Corrects grammar and punctuation, and collapses white-space in each of the input
    files, outputting to STDOUT by default.
