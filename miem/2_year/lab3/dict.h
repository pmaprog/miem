#ifndef DICT_H
#define DICT_H

#include <iostream>

typedef unsigned int Uint;

template <class T>
class Dict {
private:
    const Uint TABLE_SIZE = 100;

    // todo
    // может быть его в public?
    struct Node {
        Node(T word, Uint count = 1, Node* next = nullptr)
            : word(word), count(count), next(next) {}

        T word;
        Uint count;
        Node *next;
    };

    Node **_table;
    Uint _size;
public:
    Dict();
    ~Dict();
    // конструктор копирования
    // todo
    Dict(const Dict &);

    // метод добавления ключа
    void insert(const T &);

    // метод проверки на наличие ключа в контейнере
    bool contains(const T &);

    // метод удаления отдельного ключа
    void remove(const T &);

    // метод удаления все хранимые объекты
    void clear();

    // метод, возращающий число хранящихся объектов
    Uint size() const { return _size; }

    // класс-итератор
    // todo
    // сделать friend
    class iterator {
    private:
        Node* _cur;
        const Dict *d;
    public:
        iterator(Node *entry) : _cur(entry) {}
        iterator(Node *entry, const Dict *d) : _cur(entry), d(d) {}

        // метод, возрающий текущее значение
        T word() const;

        // метод, возрающий текущий ключ
        Uint count() const;

        // метод, перемещающий итератор вперед
        iterator& operator ++();
        iterator operator ++(int);

        bool operator ==(const iterator &) const;
        bool operator !=(const iterator &) const;
    };

    // методы, возрающие итератор для своего первого и последнего элемента
    // todo
    iterator begin() const;
    iterator end() const;

    // оператор ==, возрающий равенство коллекций по ключам (и значениям)
    // todo
    bool operator ==(const Dict &);

    // оператор [], возрающий значение по введенному ключу
    // todo
    Uint operator [](const T &);
    Uint count(const T&) const;

    // оператор ||, возвращающий новую коллекцию, содержащую все ключи обеих кол-
    // лекций и суммирующий при этом их значения
    // todo
    Dict& operator ||(const Dict &);

    // Хэш-функция, принимающую значение int и возращающую беззнаковое целое
    // todo
    // убрать из класса
    Uint hashFunc(const T&) const;

    template <class T2>
    friend std::ostream& operator <<(std::ostream &, const Dict<T2> &);

    template <class T2>
    friend std::istream& operator >>(std::istream &, const Dict<T2> &);
};

// todo
// оператор <<, отправляющий коллекцию в поток
template <class T>
std::ostream& operator <<(std::ostream &os, const Dict<T>& d) {
    return os;
}

// оператор >>, читающий коллекцию из потока
template <class T>
std::istream& operator <<(std::istream &is, const Dict<T>& d) {
    return is;
}

template <class T>
Dict<T>::Dict() : _size(0) {
    _table = new Node *[TABLE_SIZE]();
}

template <class T>
Dict<T>::~Dict() {
    clear();
    delete [] _table;
}

template <class T>
Dict<T>::Dict(const Dict& d) : Dict() {
    // эффективно?
}

template <class T>
Uint Dict<T>::hashFunc(const T& word) const {
//    Uint hash = 5381;
//    int c;

//    const char *w = word.c_str();

//    while (c = *w++)
//        hash = ((hash << 5) + hash) + c;

//    return hash % TABLE_SIZE;

    // todo
    return 1;
}

template <class T>
void Dict<T>::insert(const T &word) {
    Uint hashValue = hashFunc(word);
    Node *prev = nullptr;
    Node *entry = _table[hashValue];

    while (entry != nullptr && entry->word != word) {
        prev = entry;
        entry = entry->next;
    }

    if (entry == nullptr) {
        entry = new Node(word);
        if (prev == nullptr) {
            _table[hashValue] = entry;
        } else {
            prev->next = entry;
        }

        ++_size;
    } else {
        // слово есть в словаре, просто увеличиваем счетчик
        entry->count++;
    }
}

template<class T>
bool Dict<T>::contains(const T &word) {
    return (count(word) != 0);
}

template <class T>
void Dict<T>::remove(const T &word) {
    Uint hashValue = hashFunc(word);
    Node *prev = nullptr;
    Node *entry = _table[hashValue];

    while (entry != nullptr && entry->word != word) {
        prev = entry;
        entry = entry->next;
    }

    if (entry == nullptr) {
        // нет слова
        return;
    } else {
        if (prev == nullptr) {
            _table[hashValue] = entry->next;
        } else {
            prev->next = entry->next;
        }
        delete entry;
        _size--;
    }
}

template <class T>
Uint Dict<T>::count(const T &word) const {
    Uint hashValue = hashFunc(word);
    Node *entry = _table[hashValue];

    while (entry != nullptr) {
        if (entry->word == word) {
            return entry->count;
        }
        entry = entry->next;
    }

    // нет слова
    return 0;
}

template <class T>
void Dict<T>::clear() {
    for (Uint i = 0; i < TABLE_SIZE; ++i) {
        Node *entry = _table[i];
        while (entry != nullptr) {
            Node *prev = entry;
            entry = prev->next;
            delete prev;
        }
        _table[i] = nullptr;
    }
    _size = 0;
}

// зачем typename?
template <class T>
typename Dict<T>::iterator Dict<T>::begin() const {
    for (Uint i = 0; i < TABLE_SIZE; ++i) {
        if (_table[i] != nullptr) {
            return iterator(_table[i], this);
        }
    }

    return nullptr;
}

template <class T>
typename Dict<T>::iterator Dict<T>::end() const {
    return nullptr;
}

template <class T>
T Dict<T>::iterator::word() const {
    if (_cur != nullptr) {
        return _cur->word;
    } else {
        return nullptr;
    }
}

template <class T>
Uint Dict<T>::iterator::count() const {
    if (_cur != nullptr) {
        return _cur->count;
    } else {
        return 0;
    }
}

template<class T>
typename Dict<T>::iterator& Dict<T>::iterator::operator ++() {
}

template<class T>
typename Dict<T>::iterator Dict<T>::iterator::operator ++(int) {
    Uint index = d->hashFunc(_cur->word);

    if (_cur == nullptr) {
        std::cout << 1 << std::endl;
        return nullptr;
    }

    if (_cur->next != nullptr) {
        std::cout << 2 << std::endl;
        _cur = _cur->next;
        return *this;
    }

    for (Uint i = index+1; i < d->TABLE_SIZE; ++i) {
        if (d->_table[i] != nullptr) {
            _cur = d->_table[i];
            std::cout << 3 << std::endl;
            return *this;
        }
    }

    std::cout << 4 << std::endl;

    return nullptr;
}

template <class T>
bool Dict<T>::iterator::operator ==(const Dict<T>::iterator &it) const {
    return (_cur == it._cur);
}

template <class T>
bool Dict<T>::iterator::operator !=(const Dict<T>::iterator &it) const {
    return (_cur != it._cur);
}

template <class T>
bool Dict<T>::operator ==(const Dict &d) {

}

template<class T>
Uint Dict<T>::operator [](const T &word) {
    return count(word);
}

#endif // DICT_H
