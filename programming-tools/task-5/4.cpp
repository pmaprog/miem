#include <iostream>
#include <functional>
#include <string>
#include <map>

using namespace std;

const int start = 4, fin = 25; // Начальное и конечное числа

int main() {
    // 4 25 +1 *2 10 или 20
    // Прямой ход расчета
    int breanchCount = 0; // Счётчик просмотренных веток дерева
    function<int (int, int, string, int, int)> calc = [&calc, &breanchCount](int x, int fin, string way, int t10, int t20) {
        breanchCount++;
        if(x < fin) {
            if (x < 10) {
                t10 = 0;
            }
            if (x < 20) {
                t20 = 0;
            }
            if (x == 10) {
                t10 = 1;
            }
            if (x == 20) {
                t20 = 1;
            }
            if (x > 10 && !t10) {
                return 0;
            }
            if (x > 20 && !t20) {
                return 0;
            }
            return calc(x+1, fin, way + " +1", t10, t20) + calc(x*2, fin, way + " *2", t10, t20);
        } else if(x == fin && t10 && t20) {
            cout << start << way << endl;
            return 1;
        }
        return 0; // if x > fin
    };

    cout << "Forward" << endl;
    int count = calc(start, fin, "", 0, 0);
    cout << "Count: " << count;
    cout << "\nbreanchCount:" << breanchCount << endl;

    // Расчет обратным ходом
    breanchCount = 0;
    function<int (int, int, string, int, int)> calcBack = [&calcBack, &breanchCount](int x, int fin, string way, int t10, int t20) {
        breanchCount++;
        if (x > fin) {
            if (x > 10) {
                t10 = 0;
            }
            if (x > 20) {
                t20 = 0;
            }
            if (x == 10) {
                t10 = 1;
            }
            if (x == 20) {
                t20 = 1;
            }
            if (x < 10 && !t10) {
                return 0;
            }
            if (x < 20 && !t20) {
                return 0;
            }
            int t = calcBack(x-1, fin, "+1 "+way, t10, t20);
            if (x % 2 == 0) t += calcBack(x/2, fin, "*2 "+way, t10, t20);
            return t;
        } else if (x == fin && t10 && t20) {
            cout << way << endl;
            return 1;
        }
    };

    cout << "\nBack\n";
    count = calcBack(fin, start, "", 0, 0);
    cout << "Count: " << count;
    cout << "\nbreanchCount:" << breanchCount << "\n";

    // Расчет методом исследования операций
    breanchCount = 0;
    map<int,int> Space;
    function<int (int, int, int, int)> calcOp = [&Space, &calcOp, &breanchCount](int x, int fin, int t10, int t20) {
        breanchCount++;
        if (x < 10) {
            t10 = 0;
        }
        if (x < 20) {
            t20 = 0;
        }
        if (x == 10) {
            t10 = 1;
        }
        if (x == 20) {
            t20 = 1;
        }
        if (x > 10 && !t10) {
            return 0;
        }
        if (x > 20 && !t20) {
            return 0;
        }
        if (Space.count(x)) return Space[x];
        if (x == fin && t10 && t20) {
            Space[x] = 1;
            return 1;
        }
        if (x < fin) {
            Space[x] = calcOp(x+1, fin, t10, t20) + calcOp(x*2, fin, t10, t20);
            return Space[x];
        }
        return 0;
    };

    cout << "\nOP\nCount: " << calcOp(start, fin, 0, 0) << "\n";
    cout << "BreanchCount: " << breanchCount << "\n";
    // Первая колонка - числа, через которые проходит путь. Вторая колонка - количество путей, которые проходят через это число
    for(auto i: Space)
        cout<< i.first << ": " << i.second << "\n";
    return 0;
}