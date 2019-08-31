#ifndef DICT_H
#define DICT_H

#include <sstream>
#include <iostream>

#include <QDebug>

using namespace std;

typedef unsigned int Uint;

extern Uint hashFunc(string);
extern Uint hashFunc(int);

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
    double _loadFactor;

    void resize(Uint newTableSize) {
        Dict result(newTableSize);

        for (auto i = begin(); i != end(); ++i)
            result.insert(i.word(), i.count());

        *this = result;
    }
public:
    // конструктор по умолчанию
    Dict(Uint tableSize = 100) : _size(0), _loadFactor(0),
        _tableSize(tableSize) {
        _table = new Node *[_tableSize]();
    }

    // деструктор
    ~Dict() {
        clear();
        delete [] _table;
    }

    // конструктор копирования
    Dict(const Dict& d) : Dict(d._tableSize) {
        for (auto i = d.begin(); i != d.end(); ++i) {
            insert(i.word(), i.count());
        }
    }

    Dict& operator =(const Dict& d) {
        if (this == &d)
            return *this;

        clear();
        delete [] _table;
        _tableSize = d._tableSize;
        _table = new Node*[_tableSize]();
        for (auto i = d.begin(); i != d.end(); ++i) {
            insert(i.word(), i.count());
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
                _loadFactor = (_loadFactor * _tableSize + 1) / _tableSize;
                if (_loadFactor >= 0.75)
                    resize(_tableSize * 2 + 1);
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
                if (entry->next == nullptr)
                    _loadFactor = (_loadFactor * _tableSize - 1) / _tableSize;
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
        _size = _loadFactor = 0;
    }

    // метод, возращающий число хранящихся объектов
    Uint size() const { return _size; }

    double getLoadFactor() const { return _loadFactor; }

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
                return T(nullptr);
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
    bool operator ==(const Dict &d) const {
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

    bool operator !=(const Dict &d) const {
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
            stream << it.word() << '|' << it.count() << '\n';
        }
        return stream;
    }

    // оператор >>, читающий коллекцию из потока

    friend istream& operator >>(istream &stream, Dict &d) {
        d.clear();
        string word;
        Uint count;
        string line;
        getline(stream, line);
        while (line != "") {
            size_t pos = 0, sep = line.find('|', pos);
            word = line.substr(pos, sep - pos);
            count = atoi(line.substr(sep + 1).c_str());
            stringstream ss(word);
            T w;
            ss >> w;
            d.insert(w, count);
            line.clear();
            getline(stream, line);
        }
        return stream;
    }
};

#endif // DICT_H
