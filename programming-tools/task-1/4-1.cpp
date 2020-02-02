#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> a;
    int n, count = 0;

    cin >> n;
    a.resize(n);
    for (auto &i : a) {
        cin >> i;
    }

    for (int i = 0; i < n-1; ++i) {
        if (a[i] == 0 || a[i+1] == 0) count++;
    }

    cout << "Количество пар, где хотя бы один элемент равен 0: " << count;

    return 0;
}