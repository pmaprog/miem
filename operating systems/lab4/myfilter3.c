#include <stdio.h>
#include <ctype.h>

int main(void) {
    int count = 0, c;

    while ((c = getchar()) != EOF) {
        if (isupper(c)) {
            count++;
        }
    }

    printf("%d\n", count);

    return 0;
}