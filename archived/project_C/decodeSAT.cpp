#include <bits/stdc++.h>
#define FU(i, a, b) for (int i = (a); i <= (b); i++)
using namespace std;

const int WEEK = 100, GROUP = 100, PLAYER = 100;
vector<int> contain[WEEK][GROUP];
bool played[PLAYER][PLAYER];

int numGroup, numPlayerGroup, numWeek, numPlayer; // g - p - w - x

int getVariable(int idPlayer, int idGroup, int idWeek) {
    --idPlayer; --idGroup; --idWeek;
    return 1 + idPlayer + idGroup * numPlayer + idWeek * numPlayer * numGroup;
}

vector<int> resolveVariable(int value) {
    FU(idPlayer, 1, numPlayer) {
        FU(idGroup, 1, numGroup) {
            FU(idWeek, 1, numWeek) {
                if (getVariable(idPlayer, idGroup, idWeek) == value) {
                    return {idPlayer, idGroup, idWeek};
                }
            }
        }
    }
    assert(false);
    return {-1, -1, -1};
}

int main() {
    if (fopen("inp2.txt", "r")) {
        freopen("inp2.txt", "r", stdin);
        freopen("out2.txt", "w", stdout);
    }

	cin >> numGroup >> numPlayerGroup >> numWeek;
    numPlayer = numGroup * numPlayerGroup;

    int idVariable = numPlayer * numGroup * numWeek;
    cerr << idVariable << endl;
    for (int i = 1; i <= idVariable; i++) {
        int x; cin >> x;
        if (x > 0) {
            vector<int> res = resolveVariable(x);
            contain[res[2]][res[1]].push_back(res[0]);
        }
    }

    FU(idWeek, 1, numWeek) {
        vector<bool> hasPlayed(numPlayer + 1, false);

        cout << "Week " << idWeek << ": ";
        FU(idGroup, 1, numGroup) {
            vector<int> &tmp = contain[idWeek][idGroup];
            // Check part 2
            assert(tmp.size() == numPlayerGroup);
            sort(begin(tmp), end(tmp));
            for (int x: tmp) {
                cout << x << ' ';
                assert(!hasPlayed[x]);
                hasPlayed[x] = true;
            }
            if (idGroup < numGroup) cout << "| ";
            else cout << '\n';

            // Check part 3
            for (int i = 0; i < numPlayerGroup; i++)
                for (int j = i + 1; j < numPlayerGroup; j++) {
                    int x1 = tmp[i], x2 = tmp[j];
                    if (x1 > x2) swap(x1, x2);
                    assert(!played[x1][x2]);
                    played[tmp[i]][tmp[j]] = true;
                }
        }
    }
    
	return 0;
}
