#include <bits/stdc++.h>

using namespace std;

int main() {
    cin.tie(0) -> sync_with_stdio(0);
    freopen("input_cnf/8-4-8.cnf", "r", stdin);
    freopen("temp.cnf", "w", stdout);

    string s; cin >> s >> s;
    int num_vars, num_clauses; cin >> num_vars >> num_clauses;

    vector<set<int>> clauses(num_clauses + 1);
    vector<short> val(num_vars + 1, 0);
    vector<set<int>> contain(num_vars + 1);

    for (int i = 1; i <= num_clauses; i++) {
        int x; cin >> x;
        while (x != 0) {
            if (x < 0) contain[-x].insert(-i);
            else contain[x].insert(i);
            clauses[i].insert(x);
            cin >> x;
        }
    }

    queue<int> q;
    for (int i = 1; i <= num_vars; i++) {
        if (*contain[i].begin() > 0) {
            val[i] = 1;
            q.push(i);
        }
        
        if (*contain[i].rbegin() < 0) {
            val[i] = -1;
            q.push(i);
        }
    }
    
    for (int i = 1; i <= num_clauses; i++) if (clauses[i].size() == 1) {
        int x = *clauses[i].begin();
        if (!val[abs(x)]) {
            val[abs(x)] = x > 0 ? 1 : -1;
            q.push(abs(x));
        }
    }

    int temp_clauses = num_clauses;

    auto Clean = [&](int id) {
        -- temp_clauses;
        for (int x : clauses[id]) {
            if (x < 0) contain[-x].erase(-id);
            else contain[x].erase(id);

            x = abs(x);
            if (val[x]) continue;
            assert(!contain[x].empty());

            if (*contain[x].begin() > 0) {
                val[x] = 1;
                q.push(x);
            }
            
            if (*contain[x].rbegin() < 0) {
                val[x] = -1;
                q.push(x);
            }
        }
        clauses[id].clear();
    };

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i : contain[u]) {
            if (i < 0) {
                clauses[-i].erase(-u);
                if (val[u] == -1) Clean(-i);
                else {
                    if (clauses[-i].empty()) {
                        cerr << "UNSATISFIABLE\n";
                        return 0;
                    }
                    if (clauses[-i].size() == 1) {
                        int x = *clauses[-i].begin();
                        if (!val[abs(x)]) {
                            val[abs(x)] = x > 0 ? 1 : -1;
                            q.push(abs(x));
                        }
                    }
                }

            } else {
                clauses[i].erase(u);
                if (val[u] == 1) Clean(i);
                else {
                    if (clauses[i].empty()) {
                        cerr << "UNSATISFIABLE\n";
                        return 0;
                    }
                    if (clauses[i].size() == 1) {
                        int x = *clauses[i].begin();
                        if (!val[abs(x)]) {
                            val[abs(x)] = x > 0 ? 1 : -1;
                            q.push(abs(x));
                        }
                    }
                }
            }
        }
    }

    vector<int> tmp;
    for (int i = 1; i <= num_vars; i++) if (val[i]) {
        ++temp_clauses;
        tmp.push_back(val[i] * i);
    }

    cout << "p cnf " << num_vars << ' ' << temp_clauses << '\n';
    for (int i = 1; i <= num_clauses; i++) if (clauses[i].size()) {
        for (int x : clauses[i]) cout << x << ' ';
        cout << "0\n";
    }
    for (int x : tmp) cout << x << ' ' << "0\n";

    return 0;
}