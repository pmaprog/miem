#include <iostream>
#include <vector>
#include <map>
#include <numeric>

using namespace std;

int main() {
    int n, max = -1;
    map<pair<string, string>, vector<int>> M;

    cin >> n;
    for (int i = 0; i < n; ++i) {
        string surname;
        string initials;
        cin >> surname >> initials;
        pair<string, string> k(surname, initials);
        M[k] = vector<int>(3);

        int sum = 0;
        for (int j = 0; j < 3; ++j) {
            cin >> M[k][j];
            sum += M[k][j];
        }

        if (sum > max) {
            max = sum;
        }
    }

    cout << "Ученики с максимальными баллами ЕГЭ:\n";
    for (auto i : M) {
        if (accumulate(i.second.begin(), i.second.end(), 0) == max) {
            printf("%s %s %d\n", i.first.first.c_str(), i.first.second.c_str(), max);
        }
    }

    return 0;
}