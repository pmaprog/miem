#include <bits/stdc++.h>

using namespace std;

bool FSM(char ch, int &S) {
    switch (S) {
    case 0:
        cout << S << " " << ch << endl;
        if (isalpha(ch) || ch == '_') { S = 1; return true; }
        S = -1; return false;
    case 1:
        cout << S << " " << ch << endl;
        if (isalpha(ch) || isdigit(ch) || ch == '_') { S = 1; return true; }
        if (ch == '(') { S = 2; return true; }
        S = -1; return false;
    case 2:
        cout << S << " " << ch << endl;
        if (isalpha(ch) || ch == '_') { S = 3; return true; }
        if (isdigit(ch)) { S = 4; return true; }
        S = -1; return false;
    case 3:
        cout << S << " " << ch << endl;
        if (isalpha(ch) || isdigit(ch) || ch == '_') { S = 3; return true; }
        if (ch == ')') { S = 5; return true; }
        if (ch == '+' || ch == '-' || ch == '/' || ch == '*') { S = 8; return true; }
        S = -1; return false;
    case 4:
        cout << S << " " << ch << endl;
        if (isdigit(ch)) { S = 4; return true; }
        if (ch == ')') { S = 5; return true; }
        if (ch == '.') { S = 6; return true; }
        S = -1; return false;
    case 5:
        cout << S << " " << ch << endl;
        if (ch == '\n') { S = 0; return true; }
        return true;
    case 6:
        cout << S << " " << ch << endl;
        if (isdigit(ch)) { S = 7; return true; }
        S = -1; return false;
    case 7:
        cout << S << " " << ch << endl;
        if (isdigit(ch)) { S = 7; return true; }
        if (ch == '+' || ch == '-' || ch == '/' || ch == '*') { S = 8; return true; }
        S = -1; return false;
    case 8:
        cout << S << " " << ch << endl;
        if (isdigit(ch)) { S = 10; return true; }
        if (isalpha(ch) || ch == '_') { S = 9; return true; }
        S = -1; return false;
    case 9:
        cout << S << " " << ch << endl;
        if (isalpha(ch) || isdigit(ch) || ch == '_') { S = 9; return true; }
        if (ch == ')') { S = 13; return true; }
        S = -1; return false;
    case 10:
        cout << S << " " << ch << endl;
        if (isdigit(ch)) { S = 10; return true; }
        if (ch == '.') { S = 11; return true; }
        S = -1; return false;
    case 11:
        cout << S << " " << ch << endl;
        if (isdigit(ch)) { S = 12; return true; }
        S = -1; return false;
    case 12:
        cout << S << " " << ch << endl;
        if (isdigit(ch)) { S = 12; return true; }
        if (ch == ')') { S = 13; return true; }
        S = -1; return false;
    case 13:
        cout << S << " " << ch << endl;
        if (ch == '\n') { S = 0; return true; }
        return true;
    default:
        return false;
    }
}

int main() {
    int S = 0;
    string str;
    set<int> final = {5, 13};
    bool FEnter = false;

    while(getline(cin, str), str != "") {
        if (!FEnter)
            FEnter = true;
        else
            FSM('\n', S);

        for (auto i : str)
            if (!FSM(i, S)) break;
    }

    cout << endl;
    cout << S << endl;
    cout << (final.count(S) ? "YES" : "NO") << endl;

    return 0;
}