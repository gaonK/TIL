// https://acmicpc.net/problem/1956
#include <iostream>
using namespace std;

int d[401][401];
int main() {
  int V, E;
  cin >> V >> E;

  while (E--) {
    int a, b, c;
    cin >> a >> b >> c;
    if (d[a][b] == 0 || d[a][b] > c) {
      d[a][b] = c;
    }
  }

  for (int i = 1; i <= V; i++) {
    for (int j = 1; j <= V; j++) {
      for (int k = 1; k <= V; k++) {
        if (d[i][k] != 0 && d[k][j] != 0) {
          if (d[i][k] + d[k][j] < d[i][j]) {
            d[i][j] = d[i][k] + d[k][j];
          }
        }
      }
    }
  }

  int answer = -1;
  for (int i = 1; i <= V; i++) {
    for (int k = 1; k <= V; k++) {
      if (d[i][k] != 0 && d[k][i] != 0) {
        if (answer == -1 || answer > d[i][k] + d[k][i]) {
          answer = d[i][k] + d[k][i];
        }
      }
    }
  }
  cout << answer << '\n';

  return 0;
}