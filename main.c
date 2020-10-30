#include <locale.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    for(int i = 0; i < argc; ++i)
        printf("%d:\t%s\n", i, argv[i]);
    return 0;
}
