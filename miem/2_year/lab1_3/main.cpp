#include "abstractvalue.h"
#include "purchase.h"
#include "donation.h"
#include "container.h"
#include <iostream>

#include "mainwindow.h"
#include <QApplication>

using namespace std;

void printContainer(const Container& c) {
    int k = 1;
    cout.setf(ios::fixed);
    cout.precision(2);
    for (auto i = c.begin(); i != c.end(); ++i, ++k) {
        cout << "#" << k << ": " << "Time " << (*i)->getTime() << ", amount " << (*i)->getAmount();
        if ((*i)->getType() == "Donation") {
            cout << ", name " << (dynamic_cast<Donation*>(*i))->getName();
        }
        cout << "\n";
    }
    cout << "---\n\n";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    AbstractValue *av;
    Purchase *p = new Purchase(0, 100), *p2 = new Purchase(*p);
    p2->setAmount(200);
    Donation *d = new Donation(0, 100), *d2 = new Donation(*d);
    d2->setName("Second");
    d2->setAmount(200);

    Container c;
    c.push(p);
    c.push(p2);
    c.push(d);
    c.push(d2);
    printContainer(c);

    c.write("a.txt");

    Container c2;
    c2.read("a.txt");
    av = *(++(++c2.begin()));
    av->setTime(100);
    printContainer(c2);
    c2.pop();
    c2.push(p);
    c2.push(p);
    printContainer(c2);
    c2.write("b.txt");

    return a.exec();
}
