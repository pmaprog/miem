#include <bits/stdc++.h>

using namespace std;

bool FSM(char ch, int &S) {
    switch (S) {
    case 0:
        cout << S << " " << ch << endl;
        if (ch == 'd') { S = 1; return true; }
        S = -1; return false;
    case 1:
        cout << S << " " << ch << endl;
        if (ch == 'o') { S = 2; return true; }
        S = -1; return false;
    case 2:
        cout << S << " " << ch << endl;
        if (ch == ' ') { S = 3; return true; }
        S = -1; return false;
    case 3:
        cout << S << " " << ch << endl;
        if (ch == 'u') { S = 4; return true; }
        S = -1; return false;
    case 4:
        cout << S << " " << ch << endl;
        if (ch == 'n') { S = 5; return true; }
        S = -1; return false;
    case 5:
        cout << S << " " << ch << endl;
        if (ch == 't') { S = 6; return true; }
        S = -1; return false;
    case 6:
        cout << S << " " << ch << endl;
        if (ch == 'i') { S = 7; return true; }
        S = -1; return false;
    case 7:
        cout << S << " " << ch << endl;
        if (ch == 'l') { S = 8; return true; }
        S = -1; return false;
    case 8:
        cout << S << " " << ch << endl;
        if (ch == ' ') { S = 9; return true; }
        S = -1; return false;
    case 9:
        cout << S << " " << ch << endl;
        if (isalpha(ch)) { S = 10; return true; }
        S = -1; return false;
    case 10:
        cout << S << " " << ch << endl;
        if (isalpha(ch) || isdigit(ch)) { S = 10; return true; }
        if (ch == '=') { S = 11; return true; }
        if (ch == '<') { S = 100; return true; }
        S = -1; return false;
    case 100:
        cout << S << " " << ch << endl;
        if (ch == '>') { S = 11; return true; }
        S = -1; return false;
    case 11:
        cout << S << " " << ch << endl;
        if (isdigit(ch)) { S = 12; return true; }
        S = -1; return false;
    case 12:
        cout << S << " " << ch << endl;
        if (isdigit(ch)) { S = 12; return true; }
        if (ch == ' ') { S = 13; return true; }
        S = -1; return false;
    case 13:
        cout << S << " " << ch << endl;
        if (isalpha(ch)) { S = 14; return true; }
        S = -1; return false;
    case 14:
        cout << S << " " << ch << endl;
        if (isalpha(ch) || isdigit(ch)) { S = 14; return true; }
        if (ch == '=') { S = 15; return true; }
        S = -1; return false;
    case 15:
        cout << S << " " << ch << endl;
        if (isalpha(ch)) { S = 16; return true; }
        if (isdigit(ch)) { S = 17; return true; }
        S = -1; return false;
    case 16:
        cout << S << " " << ch << endl;
        if (isalpha(ch) || isdigit(ch)) { S = 16; return true; }
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') { S = 18; return true; }
        if (ch == ' ') { S = 21; return true; }
        S = -1; return false;
    case 17:
        cout << S << " " << ch << endl;
        if (isdigit(ch)) { S = 17; return true; }
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') { S = 18; return true; }
        if (ch == ' ') { S = 21; return true; }
        S = -1; return false;
    case 18:
        cout << S << " " << ch << endl;
        if (isalpha(ch)) { S = 19; return true; }
        if (isdigit(ch)) { S = 20; return true; }
        S = -1; return false;
    case 19:
        cout << S << " " << ch << endl;
        if (isalpha(ch) || isdigit(ch)) { S = 19; return true; }
        if (ch == ' ') { S = 21; return true; }
        S = -1; return false;
    case 20:
        cout << S << " " << ch << endl;
        if (isdigit(ch)) { S = 20; return true; }
        if (ch == ' ') { S = 21; return true; }
        S = -1; return false;
    case 21:
        cout << S << " " << ch << endl;
        if (ch == 'l') { S = 22; return true; }
        S = -1; return false;
    case 22:
        cout << S << " " << ch << endl;
        if (ch == 'o') { S = 23; return true; }
        S = -1; return false;
    case 23:
        cout << S << " " << ch << endl;
        if (ch == 'o') { S = 24; return true; }
        S = -1; return false;
    case 24:
        cout << S << " " << ch << endl;
        if (ch == 'p') { S = 25; return true; }
        S = -1; return false;
    case 25:
        cout << S << " " << ch << endl;
        return true;
    default:
        return false;
    }
}

int main() {
    int S = 0;
    set<int> final = {25};

    string str;
    getline(cin, str);

    for (auto i : str)
        if (!FSM(i, S))
            break;

    cout << endl;
    cout << S << endl;
    cout << (final.count(S) ? "YES" : "NO") << endl;

    return 0;
}