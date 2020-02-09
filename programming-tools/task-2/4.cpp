#include <iostream>
#include <vector>
#include <map>
#include <numeric>

using namespace std;

int main() {
    int n;
    // в качестве ключа словаря будем использовать логин, в качестве значения
    // массив из двух элементов: 1 элемент - фамилия, 2 элемент - IP
    map< string, vector<string> > M;
    string subnet;

    cin >> subnet;
    cin >> n;

    // читаем данные
    for (int i = 0; i < n; ++i) {
        string surname, login, ip;
        cin >> surname >> login >> ip;
        M[login] = {surname, ip}; // т.е. M[login][0] = surname, M[login][1] = ip
    }

    // перебираем словарь
    for (auto i : M) {
        // если начало IP совпадает с subnet -> выводим на экран
        // i.first - вернет ключ,
        // i.second - вернет значение (значение в нашем случае это массив)
        if (i.second[1].rfind(subnet.c_str(), 0) == 0) {
            cout << i.first << ' ' << i.second[1] << endl;
        }
    }

    return 0;
}