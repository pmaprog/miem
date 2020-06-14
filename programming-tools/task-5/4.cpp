#include <iostream>
#include <functional>
#include <string>
#include <map>

using namespace std;

const int start = 4, finish = 25;

int main() {
    int callCount = 0;
    map<int, int> space;
    function<int (int, int, int, int)> calc = [&calc, &callCount, &space](int x, int y, int t10, int t20) {
        callCount++;

        if (!t10 && x == 10) t10 = 1;
        if (!t20 && x == 20) t20 = 1;
        if (x > 20 && !t10 && !t20) return 0;

        if (space.count(x)) return space[x];
        if (x == y && (t10 || t20)) {
            space[x] = 1;
            return 1;
        }
        if (x < y) {
            space[x] = calc(x+1, finish, t10, t20) + calc(x*2, finish, t10, t20);
            return space[x];
        }

        return 0;
    };

    int count = calc(start, finish, 0, 0);
    cout << "Количество способов: " << count << endl;
    cout << "Количество вызовов: " << callCount << endl;

    cout << "Вектор фазового пространства:\n";
    for (auto i : space)
        cout << i.first << " " << i.second << endl;

    return 0;
}