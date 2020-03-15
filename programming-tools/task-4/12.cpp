#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


int main() {
    int n = 3, k = 1;
    vector<string> a;
    string sub, sentence;

    while (1) {
        string s;
        cin >> s;
        if (s == "~") break;
        a.push_back(s);
    }

    cout << "Исходный массив:\n";
    for (auto i : a)
        cout << i << "\n";

    cout << "\n";

    for (auto &s : a) {
        int pos = s.find("*");
        if (pos != -1) {
            s.erase(s.begin() + pos, s.end());
        }

        if (k + n <= s.length()) {
            sub.append(s.substr(k, n));
        }
    }

    if (sub.length()) {
        for (auto s : a) {
            if (s.length() > n) {
                string t = s;
                reverse(t.begin(), t.begin()+n); 
                reverse(t.begin()+n, t.end()); 
                reverse(t.begin(), t.end()); 
                sentence.append(t + " ");
            }
        }
    }

    cout << "Массив после изменений:\n";
    for (auto s : a)
        cout << s << "\n";

    cout << "\n";
    cout << "Подстрока: " << sub << "\n";
    cout << "Предложение: " << sentence << "\n";

    return 0;
}
