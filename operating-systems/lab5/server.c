#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "server.h"

int main(int argc, char **argv) {
    int n, fdpub, fdpriv, fd;
    struct message msg; /*структура сообщения, которое читается из общедоступного именованного канала*/
    char line[LINESIZE];

    while (1) {
        /* Общедоступный канал открывается для чтения. Системный вызов open() блокируется, если никакой процесс-клиент не открывает другой конец канала*/
        if ((fdpub = open(PUBLIC, O_RDONLY)) == -1) {
            perror(PUBLIC);
            exit(1);
        }

        /* Из общедоступного канала читается сообщение, содержащее имена личного канала и */
        /* требуемого файла. Требуемый файл открывается для чтения. */
        if (read(fdpub, (char*)&msg, sizeof(msg)) == -1) {
            perror("Reading msg from client");
            exit(1);
        }

        if ((fd = open(msg.filename, O_RDONLY)) == -1) {
            perror(msg.filename);
            exit(1);
        }

        /* Личный канал открывается для записи. Файловый сервер спит, пока клиент не откроет свой конец личного канала. Если возникли
        проблемы, и клиент не может открыть канал, сервер повиснет внутри этого open(). !см. соответствующие ключи, предотвращающие блокировки. */
        if ((fdpriv = open(msg.privfifo, O_WRONLY)) == -1) {
            perror(msg.privfifo);
            exit(1);
        }

        /* данные из файла копируются в личный именованный канал */
        while((n = read(fd, line, LINESIZE)) > 0) {
            write(fdpriv, line, n);
        }

        /* Когда копирование закончено, требуемый файл и личный именованный канал закрывается */
        close(fd);
        close(fdpriv);

        /* Закрыть общедоступный канал. Сервер переходит к следующему витку цикла. Причина для закрытия и повторного открытия состоит в том, что хочется, чтобы файловый сервер спал на open(), ожидая запросов. */
        close(fdpub);
    }

    return 0;
}
