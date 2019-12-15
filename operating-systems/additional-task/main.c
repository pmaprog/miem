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


int main(int argc, char *argv[]) {
    int semid, p[2], sent = 0, received = 0;
    char path[] = "main.c";
    key_t key;
    char msg[128];

    struct sembuf write_sem_take_1 = {0, -1},
                  write_sem_take_2 = {0, -2, IPC_NOWAIT},
                  write_sem_put_3 = {0, +3},
                  write_sem_wait = {0, 0},

                  read_sem_take_1 = {1, -1},
                  read_sem_take_2 = {1, -2},
                  read_sem_put_1 = {1, +1},
                  read_sem_put_3 = {1, +3},
                  read_sem_wait = {1, 0};

    if ((key = ftok(path, 2309)) < 0) {
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
    
    case 0:
        while(sent != 6 || received != 6) {
            if (sent != 6) {
                if (semop(semid, &write_sem_take_2, 1) == 0) { // если удалось захватить семафор на запись
                    sent++;
                    snprintf(msg, sizeof msg, "child #%d msg\n", sent);
                    write(p[1], msg, sizeof msg);

                    semop(semid, &write_sem_take_1, 1);
                    semop(semid, &read_sem_wait, 1);
                    semop(semid, &read_sem_put_3, 1);
                } else if (errno != EAGAIN) {
                    perror("semop()");
                    return -1;
                }
            }
            
            if (received != 6 && errno == EAGAIN) { // не удалось захватить
                received++;
                semop(semid, &read_sem_take_2, 1);
                semop(semid, &write_sem_wait, 1);

                read(p[0], msg, sizeof msg);
                printf("child: %s", msg);
                fflush(stdout);

                semop(semid, &read_sem_take_1, 1);
                semop(semid, &write_sem_put_3, 1);
            }

            sleep(1);
        }

        return EXIT_SUCCESS;
    }

    while(sent != 6 || received != 6) {
        if (sent != 6) {
            if (semop(semid, &write_sem_take_2, 1) == 0) { // если удалось захватить семафор на запись
                sent++;
                snprintf(msg, sizeof msg, "parent #%d msg\n", sent);
                write(p[1], msg, sizeof msg);

                semop(semid, &write_sem_take_1, 1);
                semop(semid, &read_sem_wait, 1);
                semop(semid, &read_sem_put_3, 1);
            } else if (errno != EAGAIN) {
                perror("semop()");
                return -1;
            }
        }

        if (received != 6 && errno == EAGAIN) { // не удалось захватить
            received++;
            semop(semid, &read_sem_take_2, 1);
            semop(semid, &write_sem_wait, 1);

            read(p[0], msg, sizeof msg);
            printf("parent: %s", msg);
            fflush(stdout);

            semop(semid, &read_sem_take_1, 1);
            semop(semid, &write_sem_put_3, 1);
        }

        sleep(1);
    }

    return EXIT_SUCCESS;
}
