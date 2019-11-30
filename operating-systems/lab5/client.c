#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#include "server.h"


int main(int argc, char **argv) {
    struct message msg;
    int n, fdpub, fdpriv, count = 0;
    char line [LINESIZE];

    /* синтезировать имя личного программного канала и создать этот канал с правами чтения и записи для всех процессов*/
    sprintf(msg.privfifo, "fifo%d", getpid());
    if (mkfifo(msg.privfifo, S_IFIFO | 0666) == -1){
        perror("[Client] Creating private fifo");
        exit(1);
    }

    /* общедоступный именованный канал (создается перед запуском сервера %mkfifo public) открывается на запись, и в него записываются имена личного канала и требуемого файла */
    if ((fdpub = open("public", O_WRONLY)) == -1) {
        perror("[Client] Opening public fifo");
        exit(1);
    }

    strcpy(msg.filename, argv[1]);
    write(fdpub, (char*)&msg, sizeof(msg));
    close(fdpub);

    /* личный именованный канал открывается для чтения */
    if ((fdpriv = open(msg.privfifo, O_RDONLY)) == -1) {
        perror("[Client] Opening private fifo");
        exit(1);
    }

    /* распечатать данные, полученные из личного канала */
    while((n = read(fdpriv, line, LINESIZE)) > 0) {
        int i;
        for (i = 0; i < LINESIZE; ++i) {
            if (i % 2 == 0) line[i] = toupper(line[i]);
            else line[i] = tolower(line[i]);
        }

        write(1, line, n);
    }

    /* личный именованный канал закрывается и удаляется из текущего каталога */
    close(fdpriv);
    unlink(msg.privfifo);

    return 0;
}
