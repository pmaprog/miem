// Организуйте двустороннее поочередное взаимодействие процесса-родителя и процесса-ребенка
// через неименованный программный канал,
// используя для синхронизации семафоры.

// Процессы взаимодействуют через единственный неименованный программный канал.
// Обмен данными организован в обе стороны.
// Необходимо произвести несколько циклов обмена (например, 5).


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {
    int sem_id, p[2], i;
    char path[] = "main.c";
    key_t key;
    struct sembuf P = {0, -1, SEM_UNDO};
    struct sembuf V = {0, +1, SEM_UNDO};
    char msg[128];

    if ((key = ftok(path, 0)) < 0) {
        perror("ftok()");
        return -1;
    }

    if ((sem_id = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        perror("semget()");
        return -1;
    }

    if (pipe(p) < 0) {
        perror("pipe()");
        return -1;
    }

    if (fcntl(p[0], F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl()");
        return -1;
    }

    if (semop(sem_id, &V, 1) < 0) {
        perror("semop()");
        return -1;
    }

    FILE *r = fdopen(p[0], "r");
    FILE *w = fdopen(p[1], "w");

    switch (fork()) {
    case -1:
        perror("fork()");
        return -1;
    
    case 0:
        for (i = 1; i <= 5; ++i) {
            puts("Child: waiting for semaphore.");
            fflush(stdout);

            semop(sem_id, &P, 1);

            puts("Child: semaphore is mine.");
            fflush(stdout);

            if (fgets(msg, sizeof(msg), r) != NULL) {
                printf("child: %s\n", msg);
                fflush(stdout);
            }

            fprintf(w, "[%d] hello from child", i);
            fflush(w);

            semop(sem_id, &V, 1);

            puts("Child: semaphore released.");
            fflush(stdout);

            sleep(1);
        }

        if (fgets(msg, sizeof(msg), r) != NULL) {
            printf("child: %s\n", msg);
            fflush(stdout);
        }

        fclose(w);
        fclose(r);
        return 0;
    }

    for (i = 1; i <= 5; ++i) {
        puts("Parent: waiting for semaphore.");
        fflush(stdout);

        semop(sem_id, &P, 1);

        puts("Parent: semaphore is mine.");
        fflush(stdout);

        if (fgets(msg, sizeof(msg), r) != NULL) {
            printf("parent: %s\n", msg);
            fflush(stdout);
        }

        fprintf(w, "[%d] hello from parent", i);
        fflush(w);

        semop(sem_id, &V, 1);

        puts("Parent: semaphore released.");
        fflush(stdout);

        sleep(1);
    }

    if (fgets(msg, sizeof(msg), r) != NULL) {
        printf("parent: %s\n", msg);
        fflush(stdout);
    }

    fclose(w);
    fclose(r);
    return 0;
}
