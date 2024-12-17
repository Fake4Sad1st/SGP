#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void backtrack(ll pos, const ll n, vector<ll>& chosen, const ll k) {
	if (chosen.size() >= k) {
		for (const auto& used : chosen) {
			cout << used << " ";
		}
		cout << "0";
		cout << "\n";
		return;
	}
	for (ll i = pos; i <= n; ++ i) {
		// if the total is not enough k elements from i
		if (chosen.size() + (n - i + 1) < k) {
			break;
		}
		chosen.push_back(i);
		backtrack(i + 1, n, chosen, k);
		chosen.pop_back();
	}
}

int64_t comb(ll n, ll k) {
	int64_t res = 1;
	if (k * 2 < n) k = n - k;
	int64_t temp;
	for (ll i = n; i >= k + 1; -- i) {
		if (1e18 / res < i) {
			cerr << "error in comb(" << n << ", " << k << ")\n";
			cerr << "propably overflow\n"; 
		}
		res *= i;
	}
	for (ll i = n; i >= k + 1; -- i) {
		res /= (n - i + 1);
	}
	return res;
}

int main() {
	freopen("data.txt", "r", stdin);
	freopen("orz.cnf", "w", stdout);
	ll n, k;
	cin >> n >> k;
	vector<ll> chosen;
	cout << "p cnf " << n << " " << comb(n, n - k + 1) << "\n";
	// At least k using binomial
	/**
	* Direct binomial combination generation
	* At leask k true <=> every comb(n, n - k + 1) has at least 1 true
	*/
	backtrack(1, n, chosen, n - k + 1);

	// At most k using sequential counter
	vector<vector<int>> mapRegister;
	mapRegister.resize(n, vector<int>(k + 1, 0));
	int counter = n;
	for (int i = 1; i < n; i++) {
		for (int j = 1; j <= k; j++) {
			mapRegister[i][j] = ++counter;
		}
	}
	ll clauses = comb(n, n - k + 1);
	// (1): If a bit is true, the first bit of the corresponding register is true
	for (int i = 1; i < n; i++) {
		cout << -i << ' ' << mapRegister[i][1] << " 0\n";
		++clauses;
	}
	// (2): All bits from 2 to k of the first register are false
	for (int j = 2;	j <= k; j++) {
		cout << -mapRegister[1][j] << " 0\n";
		++clauses;
	}
	// (3): If R[i - 1][j] = 1, R[i][j] = 1;
	for (int i = 2; i < n; i++) {
		for (int j = 1; j <= k; j++) {
			cout << -mapRegister[i - 1][j] << ' ' << mapRegister[i][j] << " 0\n";
			++clauses;
		}
	}
	// (4): If bit i is on and R[i - 1][j - 1] = 1, R[i][j] = 1;
	for (int i = 2; i < n; i++) {
		for (int j = 2; j <= k; j++) {
			cout << -i << ' ' << -mapRegister[i - 1][j - 1] << ' ' << mapRegister[i][j] << " 0\n";
			++clauses;
		}
	}
	// (5): At most k bits are on
	for (int i = 2; i <= n; i++) {
		cout << -i << ' ' << -mapRegister[i - 1][k] << " 0\n";
		++clauses;
	}
	cerr << counter << ' ' << clauses << endl;
	return 0;
}