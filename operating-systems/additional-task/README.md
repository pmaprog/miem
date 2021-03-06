**sem_1.c и sem_2.c**  
Простейшие программы, синхронизирующие свои действия с помощью семафоров:
- Первая программа выполняет над семафором S операцию D(S,1).
- Вторая программа выполняет над тем же семафором операцию A(S,1).
- Если семафора в системе не существует, любая программа создает его перед выполнением операции.
- Поскольку при создании семафор всегда инициируется 0, то программа 1 может работать без блокировки только после запуска программы 2. 

**main.c**  
Исходный код выполненного задания от преподавателя
> Организуйте двустороннее поочередное взаимодействие процесса-родителя и процесса-ребенка через неименованный программный канал, используя для синхронизации семафоры. Процессы взаимодействуют через единственный неименованный программный канал. Обмен данными организован в обе стороны. Необходимо произвести несколько циклов обмена (например, 5).

Схемы работы функций, которые используют семафоры:

![piperead](./piperead_schema.png)

![pipewrite](./pipewrite_schema.png)