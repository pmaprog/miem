#include <stdio.h>
#include <ctype.h>

int main(void) {
    int i = 0, c;

    while ((c = getchar()) != EOF) {
        i++;

        if (i % 2 == 0) c = toupper(c);
        else c = tolower(c);
        putchar(c);
    }

    return 0;
}