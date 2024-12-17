#include <bits/stdc++.h>
using namespace std;

int numVariable = 0, numClause = 0;
int idVariable = 0, idClause = 0;

void addNSC(int n, int k) {
    numVariable += (n - 1) * k - k * (k - 1) / 2;
    numClause += (n - 1) + k + 2 + (n - k) + 2 * (k * (k - 1) / 2 + k * (n - k - 1))
				+ 2 * (k * (k - 1) / 2 - 1 + k * (n - k) - (n - 2));
}

void newSequentialCounter(int n, int k) {
    // Exactly k using NSC
	vector<vector<int>> mapRegister;
	mapRegister.resize(n, vector<int>(k + 1, 0));
	for (int i = 1; i < n; i++)
		for (int j = 1; j <= min(i, k); j++) mapRegister[i][j] = ++idVariable;
    
	// (1): If a bit is true, the first bit of the corresponding register is true
	for (int i = 1; i < n; i++) {
		cout << -i << ' ' << mapRegister[i][1] << " 0\n";
		++idClause;
	}
	
	// (2): If R[i - 1][j] = 1, R[i][j] = 1;
	for (int i = 2; i < n; i++) {
		for (int j = 1; j <= min(i - 1, k); j++) {
			cout << -mapRegister[i - 1][j] << ' ' << mapRegister[i][j] << " 0\n";
			++idClause;
		}
	}
	
	// (3): If bit i is on and R[i - 1][j - 1] = 1, R[i][j] = 1;
	for (int i = 2; i < n; i++) {
		for (int j = 2; j <= min(i, k); j++) {
			cout << -i << ' ' << -mapRegister[i - 1][j - 1] << ' ' << mapRegister[i][j] << " 0\n";
			++idClause;
		}
	}

    // (4): If bit i is off and R[i - 1][j] = 0, R[i][j] = 0;
    for (int i = 2; i < n; i++) {
		for (int j = 1; j <= min(i - 1, k); j++) {
			cout << i << ' ' << mapRegister[i - 1][j] << ' ' << -mapRegister[i][j] << " 0\n";
			++idClause;
		}
	}

	// (5): If bit i is off, R[i][i] = 0;
	for (int i = 1; i <= k; i++) {
		cout << i << ' ' << -mapRegister[i][i] << " 0\n";
		++idClause;
	}

    // (6): If R[i - 1][j - 1] = 0, R[i][j] = 0;
    for (int i = 2; i < n; i++) {
		for (int j = 2; j <= min(i, k); j++) {
			cout << mapRegister[i - 1][j - 1] << ' ' << -mapRegister[i][j] << " 0\n";
			++idClause;
		}
	}

    // (7): (At least p) R[n - 1][k] = 1 or (n-th bit is true and R[n - 1][k - 1] = 1)
    cout << mapRegister[n - 1][k] << ' ' << n << " 0\n";
    cout << mapRegister[n - 1][k] << ' ' << mapRegister[n - 1][k - 1] << " 0\n";
	idClause += 2;
    // cout << mapRegister[n - 1][k - 1] << " 0\n";
	

	// (8): (At most p) If i-th bit is true, R[i - 1][k] = 0;
	for (int i = k + 1; i <= n; i++) {
		cout << -i << ' ' << -mapRegister[i - 1][k] << " 0\n";
		++idClause;
	}
	
}

int main() {
    if (fopen("data.txt", "r")) {
        freopen("data.txt", "r", stdin);
        freopen("orz.cnf", "w", stdout);
    }
	int g, p, w; cin >> g >> p >> w;
	int n = g * p, k = p;
	assert(n >= k);
    numVariable = idVariable = n;

    addNSC(n, k);
	cout << "p cnf " << numVariable << " " << numClause << "\n";

    newSequentialCounter(n, k);
	
	cerr << numVariable << ' ' << numClause << endl;
    assert(idVariable == numVariable && idClause == numClause);
	return 0;
}
