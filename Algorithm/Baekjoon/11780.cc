// https://acmicpc.net/problem/11780
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define INF 10000001

int d[101][101];
int v[101][101];

int main() {
  cin.tie(NULL);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      d[i][j] = INF;
    }
  }

  for (int i = 0; i < m; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    a--; b--;
    if (d[a][b] > c) {
      d[a][b] = c;
      v[a][b] = b;
    }
  }

  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (i == j) continue;
        if (d[i][j] > d[i][k] + d[k][j]) {
          d[i][j] = d[i][k] + d[k][j];
          v[i][j] = v[i][k];
        }
      }
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      cout << (d[i][j] == INF ? 0 : d[i][j]) << ' ';
    }
    cout << '\n';
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (d[i][j] == INF) {
        cout << 0 << '\n';
        continue;
      }
      vector<int> path;
      path.push_back(i);
      for(int k = i; k != j;) {
        path.push_back(k = v[k][j]);
      }
      cout << path.size() << ' ';
      for (int p : path) {
        cout << p + 1 << ' ';
      }
      cout << '\n';
    }
  }

  return 0;
}