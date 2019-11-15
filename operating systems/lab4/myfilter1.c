#include <stdio.h>

int main() {
    char c;
    int i = 0, row = 0, page = 0;

    while ((c = getchar()) != EOF) {
        i++;
        if (c == '\n') c = ' ';
        putchar(c);
        if (i == 50) {
            i = 0;
            putchar('\n');
            row++;
            if (row == 24) {
                row = 0;
                page++;
                printf("<<<%d>>>\n\n", page);
            }
        }
    }

    if (i > 1) {
        page++;
        printf("\n<<<%d>>>\n", page);
    }

    return 0;
}