#include "container.h"

Container::Container() {
    _front = _rear = nullptr;
    _size = 0;
}

Container::~Container() {
    clear();
}

Container::Container(const Container& c) : Container() {
    for (Iterator i = c.begin(); i != c.end(); ++i)
        push(*i);
}

void Container::push(AbstractValue* av) {
    Item *tmp = new Item();
    tmp->data = av;
    tmp->next = nullptr;

    if (_size == 0) {
        _front = _rear = tmp;
    } else {
        _rear->next = tmp;
        _rear = tmp;
    }

    _size++;
}

void Container::pop() {
    if (_size != 0) {
        Item *item = _front;
        _front = _front->next;
        delete item;
        _size--;
    }
}

void Container::clear() {
    while (_size != 0) {
        pop();
    }
}

unsigned int Container::size() const { return _size; }

bool Container::operator ==(const Container& c) const {
    if (_size != c._size) {
        return false;
    } else {
        Iterator i = begin(), j = c.begin();
        for (; i != end(); ++i, ++j) {
            if ((*i)->getType() == (*j)->getType()) {
                // Вызываем подходящий оператор сравнения.
                if ((*i)->getType() == "Purchase") {
                    if (*dynamic_cast<Purchase*>(*i) != *dynamic_cast<Purchase*>(*j)) break;
                } else if ((*i)->getType() == "Donation") {
                    if (*dynamic_cast<Donation*>(*i) != *dynamic_cast<Donation*>(*j)) break;
                }
            } else return false;
        }
        return (i == end());
    }
}

bool Container::operator !=(const Container& c) const {
    return !(*this == c);
}

double Container::calculateRevenue(const time_t t1, const time_t t2) const {
    double sum = 0;
    for (Iterator i = begin(); i != end(); ++i) {
        if ((*i)->getType() == "Purchase" && (*i)->getTime() >= t1 && (*i)->getTime() <= t2) {
            sum += (*i)->getAmount();
        }
    }
    return sum;
}

void Container::write(string filename) const {
    ofstream fout(filename);
    fout.setf(ios::fixed);
    fout.precision(2);

    for (Iterator i = begin(); i != end(); ++i) {
        fout << (*i)->getType() << " " << (*i)->getTime() << " " << (*i)->getAmount();
        if ((*i)->getType() == "Donation") {
            fout << " " << dynamic_cast<Donation*>(*i)->getName();
        }
        if (i != Iterator(_rear)) {
            fout << "\n";
        }
    }

    fout.close();
}

// todo
// проверка
bool Container::read(string filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        return false;
    }

    while (!fin.eof()) {
        time_t time; double amount; std::string name, type;
        AbstractValue *av = nullptr;
        fin >> type >> time >> amount;
        if (type == "Purchase") {
            av = new Purchase(time, amount);
        } else if (type == "Donation") {
            fin >> name;
            av = new Donation(time, amount, name);
        }
        push(av);
    }

    fin.close();
    return true;
}

Container::Iterator Container::begin() const {
    return _front;
}

Container::Iterator Container::end() const {
    return nullptr;
}

Container::Iterator Container::Iterator::operator ++(int) {
    Iterator old = *this;
    if (_cur != nullptr) {
        _cur = _cur->next;
    }
    return old;
}

Container::Iterator Container::Iterator::operator ++() {
    if (_cur != nullptr) {
        _cur = _cur->next;
    }
    return *this;
}

bool Container::Iterator::operator !=(const Iterator& it) const {
    return (_cur != it._cur);
}

bool Container::Iterator::operator ==(const Iterator& it) const {
    return (_cur == it._cur);
}

AbstractValue* Container::Iterator::operator *() const {
    return _cur ? _cur->data : nullptr;
}
