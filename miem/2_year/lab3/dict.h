#ifndef DICT_H
#define DICT_H

#include <iostream>

using namespace std;

typedef unsigned int Uint;

extern Uint hashFunc(int);
extern Uint hashFunc(string);

template <class T, const Uint TABLE_SIZE = 100>
class Dict {
private:
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
    // конструктор по умолчанию
    Dict() : _size(0) {
        _table = new Node *[TABLE_SIZE]();
    }

    // деструктор
    ~Dict() {
        clear();
        delete [] _table;
    }

    // конструктор копирования
    Dict(const Dict& d) : Dict() {
        for (auto it = d.begin(); it != d.end(); ++it) {
            insert(it.word(), it.count());
        }
    }

    // question: по значению или по ссылке?
    Dict& operator =(const Dict& d) {
        clear();
        for (auto it = d.begin(); it != d.end(); ++it) {
            insert(it.word(), it.count());
        }
        return *this;
    }

    // метод добавления ключа
    void insert(const T &word, Uint count = 1) {
        Uint hash = hashFunc(word);
        Node *prev = nullptr;
        Node *entry = _table[hash];

        if (count <= 0) {
            count = 1;
        }

        while (entry != nullptr && entry->word != word) {
            prev = entry;
            entry = entry->next;
        }

        if (entry == nullptr) {
            entry = new Node(word, count);

            if (prev == nullptr) {
                _table[hash] = entry;
            } else {
                prev->next = entry;
            }

            ++_size;
        } else {
            // слово есть в словаре, просто увеличиваем счетчик
            entry->count += count;
        }
    }

    // метод проверки на наличие ключа в контейнере
    bool contains(const T &word) {
        return (count(word) != 0);
    }

    // метод удаления отдельного ключа
    void remove(const T &word) {
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

    Uint count(const T &word) const {
        for (Node *i = _table[hashFunc(word)]; i != nullptr; i = i->next) {
            if (i->word == word) {
                return i->count;
            }
        }

        // нет слова
        return 0;
    }

    // метод, изменяющий значение
    void set(const T &word, Uint count) {
        if (count <= 0) {
            return;
        }

        for (Node *i = _table[hashFunc(word)]; i != nullptr; i = i->next) {
            if (i->word == word) {
                i->count = count;
                return;
            }
        }
    }

    // метод удаления все хранимые объекты
    void clear() {
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

    // метод, возращающий число хранящихся объектов
    Uint size() const { return _size; }

    // класс-итератор
    class iterator {
    private:
        Node* _cur;
        const Dict *d;
    public:
        iterator(Node *entry, const Dict *d) : _cur(entry), d(d) {}

        // метод, возрающий текущее значение
        T word() const {
            if (_cur != nullptr) {
                return _cur->word;
            } else {
                return nullptr;
            }
        }

        // метод, возрающий текущий ключ
        Uint count() const {
            if (_cur != nullptr) {
                return _cur->count;
            } else {
                return 0;
            }
        }

        // метод, перемещающий итератор вперед
        iterator& operator ++() {
            Uint index = hashFunc(_cur->word);

            if (_cur == nullptr) {
                return *this;
            }

            if (_cur->next != nullptr) {
                _cur = _cur->next;
                return *this;
            }

            for (Uint i = index+1; i < TABLE_SIZE; ++i) {
                if (d->_table[i] != nullptr) {
                    _cur = d->_table[i];
                    return *this;
                }
            }

            _cur = nullptr;
            return *this;
        }

        iterator operator ++(int) {
            iterator old = *this;
            ++(*this);
            return old;
        }

        bool operator ==(const iterator &it) const {
            return (_cur == it._cur);
        }

        bool operator !=(const iterator &it) const {
            return (_cur != it._cur);
        }
    };

    // методы, возрающие итератор для своего первого и последнего элемента
    iterator begin() const {
        for (Uint i = 0; i < TABLE_SIZE; ++i) {
            if (_table[i] != nullptr) {
                return iterator(_table[i], this);
            }
        }

        return iterator(nullptr, this);
    }

    iterator end() const {
        return iterator(nullptr, this);
    }

    // оператор ==, возрающий равенство коллекций по ключам (и значениям)
    bool operator ==(const Dict &d) {
        if (_size != d._size) {
            return false;
        }

        auto i = begin(), j = d.begin();
        for (; i != end() && j != d.end(); ++i, ++j) {
            if (i.word() != j.word() || i.count() != j.count()) {
                return false;
            }
        }

        if (i == end() && j == d.end()) {
            return true;
        }
    }

    bool operator !=(const Dict &d) {
        return !(*this == d);
    }

    // оператор [], возрающий значение по введенному ключу
    Uint operator [](const T &word) const {
        return count(word);
    }

    // оператор ||, возвращающий новую коллекцию, содержащую все ключи обеих кол-
    // лекций и суммирующий при этом их значения
    // question: возращать по ссылке или по значению?
    Dict operator ||(const Dict &d) {
        Dict result = *this;

        for (auto it = d.begin(); it != d.end(); ++it) {
            result.insert(it.word(), it.count());
        }

        return result;
    }

    // оператор <<, отправляющий коллекцию в поток (ofstream)
    friend ostream& operator <<(ostream &s, const Dict &d) {
        int c = 0;
        for (auto it = d.begin(); it != d.end(); ++it, ++c) {
            s << it.word() << ' ' << it.count();
            if (c != d._size-1) {
                s << "\n";
            }
        }
        return s;
    }

    // оператор >>, читающий коллекцию из потока
    friend istream& operator >>(istream &s, Dict &d) {
        T word;
        Uint count;
        s >> word >> count;
        d.insert(word, count);
        return s;
    }
};

#endif // DICT_H
