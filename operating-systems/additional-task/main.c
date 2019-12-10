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
    int p[2];

    pipe(p);

    switch (fork()) {
    case -1:
        perror("Неудачный вызов fork()");
        return -1;
    
    case 0:
        return 0;
    }

    return 0;
}
