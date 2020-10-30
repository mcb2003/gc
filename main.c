/*
    gc - Grammar Correcter
    Copyright (C) 2020  Michael Connor Buchan

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Author: Michael Connor Buchan <mikeybuchan@hotmail.co.uk>
   */

/* SPDXLicenseIdentifier: GPL-3-or-later */

#include <ctype.h> // Char comparison functions
#include <errno.h> // Error handling
#include <locale.h> // Localisation
#include <stdbool.h> // Booleans
#include <stdio.h> // Input / Output
#include <stdlib.h> // Mem management, file I/O
#include <string.h> // Comparing strings
#include <unistd.h> // For isatty(), STDOUT_FILENO

// The name of this program
// This is modified by main() if it is recieved as the first command-line arg
char *G_PROG_NAME = "gc";

// Prints help / usage information
void help(bool usage) {
    if(usage) fprintf(stderr, "%s: No files specified\n", G_PROG_NAME);
        fprintf(stderr, "Usage: %s file1[ file2[ ...]]\n", G_PROG_NAME);
        if(!usage) {
            fprintf(stderr, "\n%s - Grammar Correcter  Copyright (C) 2020  Michael Connor Buchan\n", G_PROG_NAME);
            fprintf(stderr, "Corrects grammar and punctuation, and collapses white-space in each of the input\nfiles, outputting to STDOUT by default.\n");
            // The user just asked for help, this was a success
            exit(EXIT_SUCCESS);
        }
        // The user didn't provide the correct args, fail!
        exit(EXIT_FAILURE);
}

// Prints an error message and terminates
void die(const char *error) {
    // Combine the program name and error message into one string
    size_t len = strlen(G_PROG_NAME) + 2 + strlen(error) + 1;
    char *text = (char *) malloc(len);
    snprintf(text, len, "%s: %s", G_PROG_NAME, error);
    text[len - 1] = '\0';

    // Print the error to STDERR
    perror(text);

    // Terminate the program
    free(text);
    exit(errno);
}

// Returns true if output is to a terminal
bool is_tty() {
    return isatty(STDOUT_FILENO);
}

// If the output is to a terminal, print the filename in bold
void print_filename(const char *fname, int argc) {
    // No need to print if there's only one file as input
    if(is_tty() && argc > 2) {
        /**
         * "\x1b[1m" turns on bold
         * "\x1b[0m" returns text to normal
         */
        printf("\x1b[1m%s:\x1b[0m", fname);
    }
}

// The entrypoint to the program
int main(int argc, char *argv[]) {
    // Set the program name
    G_PROG_NAME = argv[0];
    // Set locale to locale specified by the environment
    setlocale(LC_ALL, "");

    // If no files were provided, error!
    if(argc < 2) help(true);
    // If the user asked for help, output it
    if(argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
        help(false);

    // Process each file specified on the command-line
    // Starts at 1 because 0 is the prog name
    for(int i = 1; i < argc; ++i) {
        // Try to open the file
            FILE *fp = fopen(argv[i], "r");
        if(!fp) die(argv[i]); // Error reading file
        print_filename(argv[i], argc);

        bool seenTextStart = false; // have we seen the first non-space char?
        bool seenPeriod = false;
        bool seenSpace = false;
        char last = '\0'; // Character before the current one
        // Loop through each character in the file
        // This is buffered by libc, so remains quite efficient
        while(!feof(fp)) {
            char c = fgetc(fp);

            if(!feof(fp) && last == '\n') {
                print_filename(argv[i], argc);
                last = '\0';
            }

            // Strip white-space from the beginning of the text
            if(!seenTextStart && isspace(c)) continue;

            if(!seenSpace && isspace(c)) seenSpace = true;
            else if(isgraph(c)) seenSpace = false; // End of whitespace
            else if(isspace(c) && c != '\n') continue; // Collapse successive whitespace
            if(c == '.') {
                seenPeriod = true;
                putchar('.');
                continue;
            }
            // Fix lower-case letters at the start of sentences
            if((seenPeriod || !seenTextStart) && islower(c)) c = toupper(c);
            if(isgraph(c)) {
                seenTextStart = true;
                seenPeriod = false;
            }
            putchar(c);
            last = c;
        }
        fflush(stdout);
            fclose(fp);
    }
    return EXIT_SUCCESS;
}
