#include <iostream>
#include <set>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;

// функция для печати множества на экран
void print_set(const set<int> &S) {
    for (auto v : S) {
        printf("%d ", v);
    }
    cout << endl;
}

int main() {
    set<int> U, A, B;

    int a, b, K, c, d, K1, K2;

    // отсюда и до 50 строчек мы вводим данные и выводим из на экран    
    cin >> a >> b >> K >> c >> d >> K1 >> K2;

    for (int t = a; t <= b; ++t) {
        U.insert(K*t);
    }

    cout << "U: ";
    print_set(U);

    for (auto v : U) {
        if (v >= c && v <= d) {
            A.insert(v);
        }
    }

    cout << "A: ";
    print_set(A);

    for (auto v : U) {
        if (v % K1 == 0 || v % K2 == 0) {
            B.insert(v);
        }
    }

    cout << "B: ";
    print_set(B);

    cout << endl;

    // теперь выполняем операции над множествами
    // (по названию функций в принципе понятно что именно мы выполняем)
    set<int> un;
    set_union(A.begin(), A.end(), B.begin(), B.end(), inserter(un, un.begin()));
    set<int> not_un;
    set_difference(U.begin(), U.end(), un.begin(), un.end(), inserter(not_un, not_un.begin()));
    set<int> not_A;
    set_difference(U.begin(), U.end(), A.begin(), A.end(), inserter(not_A, not_A.begin()));
    set<int> not_B;
    set_difference(U.begin(), U.end(), B.begin(), B.end(), inserter(not_B, not_B.begin()));
    set<int> intsec;
    set_intersection(not_A.begin(), not_A.end(), not_B.begin(), not_B.end(), inserter(intsec, intsec.begin()));
    set<int> diff;
    set_difference(not_un.begin(), not_un.end(), intsec.begin(), intsec.end(), inserter(diff, diff.begin()));

    // diff - содержит разность двух множеств, если разность - это пустое множество
    // значит операции идентичны
    if (diff.size() == 0) {
        cout << "Операции идентичны!\n";
    } else {
        cout << "Операции не идентичны!\n";
    }
    cout << endl;

    cout << "Результат выполнения операции:\n";
    print_set(intsec);

    // вывод на экран элементов с четными индексами
    cout << "Четные по индексу элементы:\n";
    for (auto it = intsec.begin(); it != intsec.end(); ++it) {
        int index = distance(intsec.begin(), it);
        if (index % 2 == 0)
            printf("%d ", *it);
    }

    return 0;
}
