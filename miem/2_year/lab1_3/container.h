#ifndef CONTAINER_H
#define CONTAINER_H

#include "abstractvalue.h"
#include "purchase.h"
#include "donation.h"
#include <fstream>

using namespace std;

class Container {
public:
    // Класс элемента коллекции.
    struct Item {
        AbstractValue *data;
        Item *next;
    };

    // Конструктор по умолчанию.
    Container();

    // Деструктор.
    ~Container();

    // Конструктор копирования.
    Container(const Container&);

    // Добавление в конец очереди.
    void push(AbstractValue*);

    // Удаление с начала очереди.
    void pop();

    // Удаляет все хранящие объекты.
    void clear();

    // Возращает количество хранящихся объектов.
    unsigned int size() const;

    // Проверка двух коллекций на равенство.
    bool operator ==(const Container&) const;
    bool operator !=(const Container&) const;

    // Расчет выручки за заданный период времени.
    double calculateRevenue(const time_t, const time_t) const;

    // Запись в файл.
    void write(string filename) const;

    // Чтение из файла.
    void read(string filename);

    class Iterator {
    public:
        Iterator(Item *item) : _cur(item) { }

        Iterator operator ++(int);
        Iterator& operator ++();

        bool operator ==(const Iterator&) const;
        bool operator !=(const Iterator&) const;

        AbstractValue* operator *() const;
    private:
        Item *_cur;
    };

    // Итератор первого элемента.
    Iterator begin() const;

    // Итератор последний элемента.
    Iterator end() const;
private:
    // Указатели на первый и последний элемент в очереди.
    Item *_front, *_rear;

    // Количество элементов в очереди.
    unsigned int _size;
};

#endif // CONTAINER_H
