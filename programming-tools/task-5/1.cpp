#include <iostream>
#include <functional>
#include <string>

using namespace std;

const int start = 4, finish = 25;

int main() {
    int callCount = 0;
    function<int (int, int, string)> calc = [&calc, &callCount](int x, int y, string way) {
        callCount++;

        if(x < y) {
            return calc(x+1, finish, way + "+1") + calc(x*2, finish, way + "*2");
        } else if (x == y) {
            cout << start << way << "=" << finish << endl;
            return 1;
        }

        return 0;
    };

    int count = calc(start, finish, "");
    cout << "Количество способов: " << count << endl;
    cout << "Количество вызовов: " << callCount << endl;

    return 0;
}