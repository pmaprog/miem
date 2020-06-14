#include <iostream>
#include <vector>

using namespace std;


int main() {
    vector<string> a;
    string s2, s3;


    while (1) {
        string s;
        cin >> s;
        if (s == "0") break;
        a.push_back(s);
    }

    cout << "Исходный массив строк: \n";
    for (auto i : a)
        cout << i << "\n";

    cout << "\n";


    for (auto &s : a) {
        int pos = 0;
        while (pos != -1 || pos < s.length()) {
            pos = s.find("ab", pos);
            if (pos != -1) {
                s.erase(pos, 2);
                s.insert(pos, "ccc"); 
            }
        }

        if (s.length() > 1)
            s2.append(s.substr(s.length() - 2));
    }


    for (auto s : a) {
        int i;
        for (i = 0; i < s2.length(); ++i) 
            if (s.find(s2[i]) != -1) break;
        if (i == s2.length()) 
            s3.append(s + " ");
    }


    cout << "После замены `ab` на `ccc`: \n";
    for (auto i : a)
        cout << i << "\n";

    cout << "\n";
    cout << "Подстрока: " << s2 << "\n";
    cout << "Предложение: " << s3 << "\n";

    return 0;
}
