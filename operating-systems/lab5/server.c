#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "server.h"


int main(int argc, char **argv) {
    int fdpub;
    FILE *fp, *fppriv;
    struct message msg; /*структура сообщения, которое читается из общедоступного именованного канала*/

    while (1) {
        /* Общедоступный канал открывается для чтения. Системный вызов open() блокируется, если никакой процесс-клиент не открывает другой конец канала*/
        if ((fdpub = open("public", O_RDONLY)) == -1) {
            perror("[Server] Opening public fifo");
            exit(1);
        }

        /* Из общедоступного канала читается сообщение, содержащее имена личного канала и */
        /* требуемого файла. Требуемый файл открывается для чтения. */
        if (read(fdpub, (char*)&msg, sizeof(msg)) == -1) {
            perror("[Server] Reading message struct");
            exit(1);
        }

        if ((fp = fopen(msg.filename, "r")) == NULL) {
            perror("[Server] Opening file");
            exit(1);
        }

        /* Личный канал открывается для записи. Файловый сервер спит, пока клиент не откроет свой конец личного канала. Если возникли
        проблемы, и клиент не может открыть канал, сервер повиснет внутри этого open(). !см. соответствующие ключи, предотвращающие блокировки. */
        if ((fppriv = fopen(msg.privfifo, "w")) == NULL) {
            perror("[Server] Opening private fifo");
            exit(1);
        }

        /* данные из файла копируются в личный именованный канал */
        int c, i = 0, row = 0, page = 0;
        while ((c = getc(fp)) != EOF) {
            i++;
            if (c == '\n') c = ' ';
            putc(c, fppriv);
            if (i == 50) {
                i = 0;
                putc('\n', fppriv);
                row++;
                if (row == 24) {
                    row = 0;
                    page++;
                    fprintf(fppriv, "<<<%d>>>\n\n", page);
                }
            }
        }

        if (i > 1) {
            page++;
            fprintf(fppriv, "\n<<<%d>>>\n", page);
        }

        /* Когда копирование закончено, требуемый файл и личный именованный канал закрывается */
        fclose(fp);
        fclose(fppriv);

        /* Закрыть общедоступный канал. Сервер переходит к следующему витку цикла. Причина для закрытия и повторного открытия состоит в том, что хочется, чтобы файловый сервер спал на open(), ожидая запросов. */
        close(fdpub);
    }

    return 0;
}
