#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(int argc, char* argv[]) {
    int semid;
    char pathname[] = "sem_1.c";
    key_t key;
    struct sembuf mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        perror("Can\'t generate key");
        return -1;
    }

    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        perror("Can\'t get semid");
        return -1;
    }

    mybuf.sem_op = -1;
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;

    if (semop(semid, &mybuf, 1) < 0) {
        perror("Can\'t wait for condition");
        return -1;
    }

    puts("Condition is present");

    return 0;
}