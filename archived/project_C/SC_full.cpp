#include <bits/stdc++.h>
using namespace std;

int numVariable = 0, numClause = 0;
int idVariable = 0, idClause = 0;

void addSC(int n, int k) {
    numVariable += (n - 1) * k;
    numClause += (n - 1) + (k - 1) + (n - 2) * k + (n - 2) * (k - 1) + (n - 1);
}

void sequentialCounter(int n, int k, int sign) {
    // At most k using sequential counter
	vector<vector<int>> mapRegister;
	mapRegister.resize(n, vector<int>(k + 1, 0));
	for (int i = 1; i < n; i++) 
		for (int j = 1; j <= k; j++) mapRegister[i][j] = ++idVariable;
    
	// (1): If a bit is true, the first bit of the corresponding register is true
	for (int i = 1; i < n; i++) {
		cout << sign * -i << ' ' << mapRegister[i][1] << " 0\n";
		++idClause;
	}

	// (2): All bits from 2 to k of the first register are false
	for (int j = 2;	j <= k; j++) {
		cout << -mapRegister[1][j] << " 0\n";
		++idClause;
	}

	// (3): If R[i - 1][j] = 1, R[i][j] = 1;
	for (int i = 2; i < n; i++) {
		for (int j = 1; j <= k; j++) {
			cout << -mapRegister[i - 1][j] << ' ' << mapRegister[i][j] << " 0\n";
			++idClause;
		}
	}

	// (4): If bit i is on and R[i - 1][j - 1] = 1, R[i][j] = 1;
	for (int i = 2; i < n; i++) {
		for (int j = 2; j <= k; j++) {
			cout << sign * -i << ' ' << -mapRegister[i - 1][j - 1] << ' ' << mapRegister[i][j] << " 0\n";
			++idClause;
		}
	}

	// (5): At most k bits are on
	for (int i = 2; i <= n; i++) {
		cout << sign * -i << ' ' << -mapRegister[i - 1][k] << " 0\n";
		++idClause;
	}
}

int main() {
    if (fopen("data.txt", "r")) {
        freopen("data.txt", "r", stdin);
        freopen("orz.cnf", "w", stdout);
    }
	int n, k; cin >> n >> k;
	assert(n >= k);
    numVariable = idVariable = n;

    addSC(n, k);
    addSC(n, n - k);
	cout << "p cnf " << numVariable << " " << numClause << "\n";

    sequentialCounter(n, k, 1); // at most k
    sequentialCounter(n, n - k, -1); // at most n - k false <=> at least k
	
	cerr << idVariable << ' ' << idClause << endl;
    assert(idVariable == numVariable && idClause == numClause);
	return 0;
}
