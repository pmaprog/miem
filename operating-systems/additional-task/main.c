// Организуйте двустороннее поочередное взаимодействие процесса-родителя и процесса-ребенка
// через неименованный программный канал,
// используя для синхронизации семафоры.

// Процессы взаимодействуют через единственный неименованный программный канал.
// Обмен данными организован в обе стороны.
// Необходимо произвести несколько циклов обмена (например, 5).


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <errno.h>

int semid, p[2], i;
key_t key;
char msg[128];

struct sembuf write_sem_take_1 = {0, -1},
              write_sem_take_2 = {0, -2},
              write_sem_wait = {0, 0},

              read_sem_take_1 = {1, -1},
              read_sem_take_2 = {1, -2},
              read_sem_wait = {1, 0},

              sems_put_3[2] = {{0, +3}, {1, +3}};



void piperead(char *msg, int length) {
    semop(semid, &read_sem_take_2, 1);
    semop(semid, &write_sem_wait, 1);

    read(p[0], msg, length);

    semop(semid, &read_sem_take_1, 1);
}


void pipewrite(const char *msg, int length) {
    semop(semid, &write_sem_take_2, 1);

    write(p[1], msg, length);

    semop(semid, &write_sem_take_1, 1);
    semop(semid, &read_sem_wait, 1);

    semop(semid, sems_put_3, 2);
}


int main(int argc, char *argv[]) {
    if ((key = ftok("main.c", 2309)) < 0) {
        perror("ftok()");
        return -1;
    }

    if ((semid = semget(key, 2, 0666 | IPC_CREAT)) < 0) {
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

    union semun u;
    u.val = 3;
    if(semctl(semid, 0, SETVAL, u) < 0 || semctl(semid, 1, SETVAL, u) < 0) {
        perror("semctl()");
        return -1;
    }

    switch (fork()) {
    case -1:
        perror("fork()");
        return -1;
    
    // child code
    case 0:
        for (i = 1; i <= 5; ++i) {
            snprintf(msg, sizeof msg, " hello from child #%d", i);
            pipewrite(msg, sizeof msg);
            piperead(msg, sizeof msg);
            printf(" CHILD: %s\n", msg);
        }

        return EXIT_SUCCESS;
    }

    // parent code
    for (i = 1; i <= 5; ++i) {
        piperead(msg, sizeof msg);
        printf("PARENT: %s\n", msg);
        snprintf(msg, sizeof msg, "hello from parent #%d", i);
        pipewrite(msg, sizeof msg);
    }

    return EXIT_SUCCESS;
}
