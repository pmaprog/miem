// ����������� ������������ ����������� �������������� ��������-�������� � ��������-�������
// ����� ������������� ����������� �����,
// ��������� ��� ������������� ��������.

// �������� ��������������� ����� ������������ ������������� ����������� �����.
// ����� ������� ����������� � ��� �������.
// ���������� ���������� ��������� ������ ������ (��������, 5).


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
        perror("��������� ����� fork()");
        return -1;
    
    case 0:
        return 0;
    }

    return 0;
}
