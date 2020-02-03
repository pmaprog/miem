#include <iostream>
#include <set>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;

template<class T>
void print_set(const set<T> &S) {
    for (auto v : S) {
        printf("%d ", v);
    }
    cout << endl;
}

int main() {
    srand(time(NULL));

    set<int> U, A, B;

    int v;
    do {
        cin >> v;
        U.insert(v);
    } while (v != 0);

    cout << "U: ";
    print_set(U);

    for (auto v : U) {
        if (v % 2 != 0) {
            A.insert(v);
        }
    }

    cout << "A: ";
    print_set(A);

    int c = rand() % (U.size()-1 - 1) + 1;
    for (int i = 0; i < c; ++i) {
        int index = rand() % (U.size()-1);
        auto it = U.begin();
        advance(it, index);
        B.insert(*it);
    }

    cout << "B: ";
    print_set(B);

    cout << endl;

    set<int> not_A;
    set_difference(U.begin(), U.end(), A.begin(), A.end(), inserter(not_A, not_A.begin()));
    set<int> not_B;
    set_difference(U.begin(), U.end(), B.begin(), B.end(), inserter(not_B, not_B.begin()));
    set<int> un;
    set_union(not_A.begin(), not_A.end(), not_B.begin(), not_B.end(), inserter(un, un.begin()));
    set<int> not_un;
    set_difference(U.begin(), U.end(), un.begin(), un.end(), inserter(not_un, not_un.begin()));
    set<int> intsec;
    set_intersection(A.begin(), A.end(), B.begin(), B.end(), inserter(intsec, intsec.begin()));
    set<int> diff;
    set_difference(not_un.begin(), not_un.end(), intsec.begin(), intsec.end(), inserter(diff, diff.begin()));

    if (diff.size() == 0) {
        cout << "Операции идентичны!\n";
    } else {
        cout << "Операции не идентичны!\n";
    }
    cout << endl;

    cout << "Результат выполнения операции:\n";
    print_set(intsec);

    cout << "30% самых маленьких элементов:\n";
    auto end = intsec.begin();
    advance(end, ceil(0.3 * intsec.size()));
    for (auto it = intsec.begin(); it != end; ++it) {
        printf("%d ", *it);
    }

    return 0;
}
