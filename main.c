#include <ctype.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t BUFF_SIZE = 512;

char *G_PROG_NAME = "gc";

// Prints an error message and terminates
void die(const char *error) {
    size_t len = strlen(G_PROG_NAME) + 2 + strlen(error) + 1;
    char *text = (char *) malloc(len);
    snprintf(text, len, "%s: %s", G_PROG_NAME, error);
    text[len - 1] = '\0';
    perror(text);
    free(text);
    exit(1);
}

int main(int argc, char *argv[]) {
    // Set the program name
    G_PROG_NAME = argv[0];
    // Set locale to locale specified by the environment
    setlocale(LC_ALL, "");

    // Check each file specified on the command line
    for(int i = 1; i < argc; ++i) {
            FILE *fp = fopen(argv[i], "r");
        if(!fp) die(argv[i]); // Error reading file
        bool seenPeriod = false;
        bool seenSpace = false;
        while(!feof(fp)) {
            char c = fgetc(fp);
            if(!seenSpace && isspace(c)) seenSpace = true;
            else if(isgraph(c)) seenSpace = false;
            else if(isspace(c)) continue;
            if(c == '.') {
                seenPeriod = true;
                printf(".");
                continue;
            }
            if(seenPeriod && islower(c)) c = toupper(c);
            if(isgraph(c)) seenPeriod = false;
            printf("%c", c);
        }
            fclose(fp);
    }
    return 0;
}