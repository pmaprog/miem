#ifndef DICT_H
#define DICT_H

#include <iostream>
#include <sstream>

using namespace std;

typedef unsigned int Uint;

extern Uint hashFunc(string);

template <class T>
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
    Uint _tableSize;
public:
    // конструктор по умолчанию
    Dict(Uint tableSize = 100) : _size(0), _tableSize(tableSize) {
        _table = new Node *[_tableSize]();
    }

    // деструктор
    ~Dict() {
        clear();
        delete [] _table;
    }

    // конструктор копирования
    Dict(const Dict& d) : Dict() {
        _tableSize = d._tableSize;
        for (auto it = d.begin(); it != d.end(); ++it) {
            insert(it.word(), it.count());
        }
    }

    Dict& operator =(const Dict& d) {
        clear();
        _tableSize = d._tableSize;
        for (auto it = d.begin(); it != d.end(); ++it) {
            insert(it.word(), it.count());
        }
        return *this;
    }

    // метод добавления ключа
    void insert(const T &word, Uint count = 1) {
        Uint h = hash(word);

        Node *prev = nullptr;
        Node *entry = _table[h];

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
                _table[h] = entry;
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
    bool contains(const T &word) const {
        return (count(word) != 0);
    }

    // метод удаления отдельного ключа
    void remove(const T &word) {
        Uint hashValue = hash(word);
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
        for (Node *i = _table[hash(word)]; i != nullptr; i = i->next) {
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

        for (Node *i = _table[hash(word)]; i != nullptr; i = i->next) {
            if (i->word == word) {
                i->count = count;
                return;
            }
        }
    }

    // метод удаления все хранимые объекты
    void clear() {
        for (Uint i = 0; i < _tableSize; ++i) {
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

    void setTableSize(Uint tableSize) {
        if (tableSize > 0) {
            _tableSize = tableSize;
        }
    }

    Uint getTableSize() const { return _tableSize; }

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
            Uint index = d->hash(_cur->word);

            if (_cur == nullptr) {
                return *this;
            }

            if (_cur->next != nullptr) {
                _cur = _cur->next;
                return *this;
            }

            for (Uint i = index+1; i < d->_tableSize; ++i) {
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
        for (Uint i = 0; i < _tableSize; ++i) {
            if (_table[i] != nullptr) {
                return iterator(_table[i], this);
            }
        }

        return iterator(nullptr, this);
    }

    iterator end() const {
        return iterator(nullptr, this);
    }

    Uint hash(const T& word) const {
        return hashFunc(word) % _tableSize;
    }

    // оператор ==, возрающий равенство коллекций по ключам (и значениям)
    bool operator ==(const Dict &d) {
        if (_size != d._size) {
            return false;
        }

        for (auto it = begin(); it != end(); ++it) {
            if (it.count() != d[it.word()]) {
                return false;
            }
        }

        return true;
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
    Dict operator ||(const Dict &d) {
        Dict result = *this;

        for (auto it = d.begin(); it != d.end(); ++it) {
            result.insert(it.word(), it.count());
        }

        return result;
    }

    // оператор <<, отправляющий коллекцию в поток (ofstream)
    friend ostream& operator <<(ostream &stream, const Dict &d) {
        int c = 0;
        for (auto it = d.begin(); it != d.end(); ++it, ++c) {
            stream << it.word() << '|' << it.count();
            if (c != d._size-1) {
                stream << "\n";
            }
        }
        return stream;
    }

    // оператор >>, читающий коллекцию из потока
    friend istream& operator >>(istream &stream, Dict &d) {
        T word;
        Uint count;
        string line;
        getline(stream, line);
        while (line != "") {
            size_t pos = 0;
            while (true) {
                size_t end = line.find('|', pos);
                if (end == line.npos) {
                    count = atoi(line.substr(pos).c_str());
                    break;
                } else {
                    word = line.substr(pos, end - pos);
                    pos = end + 1;
                }
            }
            d.insert(word, count);
            line.clear();
            getline(stream, line);
        }
        return stream;
    }
};

#endif // DICT_H
