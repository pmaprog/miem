#include <iostream>
#include <functional>
#include <string>

using namespace std;

const int start = 4, finish = 25;

int main() {
    int callCount = 0;
    function<int (int, int, string, int, int)> calc = [&calc, &callCount](int x, int y, string way, int t10, int t20) {
        callCount++;

        if(x < y) {
            if (!t10 && x == 10) t10 = 1;
            if (!t20 && x == 20) t20 = 1;
            if (x > 20 && !t10 && !t20) return 0;
            return calc(x+1, finish, way + "+1", t10, t20) + calc(x*2, finish, way + "*2", t10, t20);
        } else if (x == y && (t10 || t20)) {
            cout << start << way << "=" << finish << endl;
            return 1;
        }

        return 0;
    };

    int count = calc(start, finish, "", 0, 0);
    cout << "Количество способов: " << count << endl;
    cout << "Количество вызовов: " << callCount << endl;

    return 0;
}