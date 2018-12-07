#include "dict.h"

#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <fstream>

using namespace std;

// todo
// Хэш-функция, принимающую значение int и возращающую беззнаковое целое
Uint hashFunc(string word) {
    Uint hash = 5381;
    int c;

    const char *w = word.c_str();

    while (c = *w++)
        hash = ((hash << 5) + hash) + c;

    return hash;
}

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();

    Dict<string> d;

    // конструктор по умолчанию: (размер коллекции == 0)
    assert(d.size() == 0);

    // метод добавления объекта: (при добавлении нового объекта размер коллекции
    // увеличивается на единицу, а при добавлении имеющегося объекта
    // размер не изменяется)
    d.insert("Word 1");
    assert(d.size() == 1);
    d.insert("Word 1");
    assert(d.size() == 1);

    // метод проверки на наличие ключа: (в коллекции присутствует добавленный объект)
    assert(d.contains("Word 1") == true);

    // метод удаления объекта: (при удалении существующего объекта размер коллекции уменьшается на
    // единицу, а при удалении отсутствующего объекта размер не изменяется)
    d.remove("Word 1");
    assert(d.size() == 0);
    Uint prevSize = d.size();
    d.remove("Word 2");
    assert(d.size() == prevSize);

    // метод проверки на наличие ключа: (в коллекции отсутствует удалённый объект)
    assert(d.contains("Word 1") == false);

    // метод удаления всех объектов: (непустая коллекция после удаления всех объектов становится равна
    // коллекции, созданной при помощи конструктора по умолчанию)
    d.insert("Word 1", 2);
    d.insert("Word 2", 2);
    d.clear();
    assert(d == (Dict<string>()));

    // (варианты 4–15) оператор []: (значение, связанное с добавленным ключом, равно ожидаемому)
    d.insert("Word 1", 2);
    d.insert("Word 1", 2);
    d.insert("Word 2", 3);
    d.insert("Word 3");
    d.insert("Word 4");
    d.insert("Word 5", 10);
    assert(d["Word 1"] == 4);
    assert(d["Word 2"] == 3);

    // конструктор копирования: (копия непустой коллекции равна оригиналу, после вставки/удаления
    // объекта (варианты 1–3) или изменения одного из значений (варианты 4–15) равенство нарушается)
    Dict<string> d2(d);
    assert(d2 == d);
    d2.set("Word 1", 10);
    assert(d2 != d);

    // операторы << и >>: (проверка, аналогичная конструктору копирования)
    ofstream out("data.txt");
    out << d;
    out.close(); // question: почему без этого не работает?
    ifstream in("data.txt");
    d2.clear();
    in >> d2;
    in.close();
    assert(d == d2);

    // Работа итератора проверяется выводом содержимого непустой коллекции на экран.
    for (auto it = d2.begin(); it != d2.end(); ++it) {
        cout << it.word() << ' ' << it.count() << "\n";
    }

    // (варианты 4–6) при объединении двух не совпадающих коллекций, оператор || возвращает кол-
    // лекцию, размер которой равен сумме размеров слагаемых; при объединении двух идентичных кол-
    // лекций, возвращает не равную им (при ненулевых значениях) новую коллекцию того же размера
    d2.clear();
    d2.insert("Word 201", 20);
    d2.insert("Word 333", 99);
    Dict<string> d3 = d || d2;
    assert(d3.size() == d.size() + d2.size());
    d3.remove("Word 201");
    d3.remove("Word 333");
    // now d3 == d
    d3 = d3 || d;
    assert(d3.size() == d.size());

    return 0;
}
