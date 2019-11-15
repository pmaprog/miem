#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(void) {
    int p[2];

    pipe(p);

    switch(fork()) {
    case -1:
        printf("Ошибка при вызове fork()\n");
        exit(1);

    case 0:
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);

        execl("myfilter2", "", NULL);
        puts("Ошибка при вызове myfilter2 \n");
        exit(1);

    default:
        close(1);
        dup(p[1]);
        close(p[1]);
        close(p[0]);

        execl("myfilter1", "", NULL);
        puts("Ошибка при вызове myfilter1\n");
        exit(1);
    }

    return 0;
}