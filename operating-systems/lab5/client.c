#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "server.h"

int main(int argc, char **argv) {
    struct message msg;
    int n, fdpub, fdpriv;
    char line [LINESIZE];

    /* синтезировать имя личного программного канала и создать этот канал с правами чтения и записи для всех процессов*/
    sprintf(msg.privfifo, "Fifo%d", getpid());
    if (mkfifo(msg.privfifo, S_IFIFO | 0666) == -1){
        perror(msg.privfifo);
        exit(1);
    }

    /* общедоступный именованный канал (создается перед запуском сервера %mkfifo public) открывается на запись, и в него записываются имена личного канала и требуемого файла */
    if ((fdpub = open(PUBLIC, O_WRONLY)) == -1) {
        perror(PUBLIC);
        exit(2);
    }

    strcpy(msg.filename, argv[1]);
    write (fdpub, (char*)&msg, sizeof(msg));

    /* личный именованный канал открывается для чтения */
    if ((fdpriv = open(msg.privfifo, O_RDONLY)) == -1) {
        perror(msg.privfifo);
        exit(3);
    }

    /*распечатать данные, полученные из личного канала */
    while((n = read(fdpriv, line, LINESIZE)) > 0) {
        write(1, line, n);
    }

    /*личный именованный канал закрывается и удаляется из текущего каталога */
    close(fdpriv);
    unlink(msg.privfifo);

    return 0;
}
