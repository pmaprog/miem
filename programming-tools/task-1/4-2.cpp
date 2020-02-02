#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


void print_M(const vector<vector<float>> &M) {
    for (auto row : M) {
        for (auto val : row)
            printf("%8.3f", val);
        cout << endl;
    }
}


int main() {
    vector< vector<float> > M;
    int n, m;
    float a, b, d, a2, b2;

    cin >> n >> m >> a >> b >> d >> a2 >> b2;

    M.resize(n, vector<float>(m));
    for (auto &i : M) {
        for (auto &val : i)
            cin >> val;
    }

    cout << "Исходные данные: \n";
    printf("a = %.6f\nb = %.6f\nd = %.6f\n\n", a, b, d);
    print_M(M);

    for (int col = m-1; col >= 0; --col) {
        int zero_count = 0, last = -1;

        for (int row = 0; row < n; ++row) {
            if (M[row][col] == 0) {
                last = row;
                ++zero_count;
            }
        }

        if (zero_count != 0 && zero_count % 2 == 0) {
            vector<float> tmp;
            for (int i = last+1; i < n; ++i) {
                tmp.push_back(M[i][col]);
            }
            tmp.push_back(0);
            for (int i = 0; i < last; ++i) {
                tmp.push_back(M[i][col]);
            }
            for (int i = 0; i < n; ++i) {
                M[i][col] = tmp[i];
            }
            break;
        }
    }

    cout << "Меняем местами части массива относительно последнего нуля: \n";
    print_M(M);

    for (auto &row : M) {
        auto it = row.begin();
        while (it != row.end()) {
            if (*it >= a && *it <= b) {
                row.erase(it);
                row.push_back(d);
                it = row.begin();
            } else ++it;
        }
    }

    cout << "Заменяем все элементы в диапазоне от a до b на d и помещаем в конец строки:\n";
    print_M(M);

    sort(M[0].begin(), M[0].end(),
        [](float a, float b) { return a > b; }
    );

    cout << "Сортируем первую строку: \n";
    print_M(M);

    for (auto &row : M) {
        float min = row[0];
        for (auto it = row.begin(); it != row.end(); ++it) {
            if (*it >= 0 && *it < min) {
                min = *it;
            }
        }
        row.push_back(min);
    }

    cout << "Добавляем новый столбец с минимальными положительными элементами каждой строки:\n";
    print_M(M);

    vector<int> count(M.size(), 0);
    for (auto row : M) {
        for (auto it = row.begin(); it != row.end(); ++it) {
            if (*it >= a2 && *it <= b2) {
                int index = distance(row.begin(), it);
                count[index]++;
            }
        }
    }
    auto min_count = min_element(count.begin(), count.end());
    M.erase(M.begin() + distance(count.begin(), min_count));

    cout << "Удаляем из матрицы строку, в которой меньше всего элементов из диапазона [a2; b2]:\n";
    print_M(M);

    return 0;
}
