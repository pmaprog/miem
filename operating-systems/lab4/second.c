#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define R 0
#define W 1

int main(void) {
    int pid, p[2], q[2], c, total = 0, i = 0;
    FILE *fp;

    pipe(p);
    pipe(q);

    switch(pid = fork()) {
    case -1:
        printf("Ошибка при вызове fork() \n");
        exit(1);

    case 0:
        dup2(p[R], R);
        close(p[R]);
        close(p[W]);

        dup2(q[W], W);
        close(q[R]);
        close(q[W]);

        execl("myfilter3", "", 0);
        printf("Ошибка при вызове myfilter3");
        exit(1);

    default:
        close(p[R]);
        close(q[W]);

        fp = fdopen(p[W], "w");
        while((c = getchar()) != EOF) {
            ++i;

            if (i % 2 == 0) c = toupper(c);
            else c = tolower(c);
            putc(c, fp);
        }
        fclose(fp);

        clearerr(stdin);

        dup2(q[R], R);
        close(q[R]);

        scanf("%d", &total);
        printf("Число символов в верхнем регистре: %d\n", total);
        exit(0);
    }
}
