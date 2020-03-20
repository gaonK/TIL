// https://acmicpc.net/problem/17435
#include <iostream>
using namespace std;

const int MAX = 500001;
const int MAX_D = 19;

int f[MAX][MAX_D];
int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int M;
  cin >> M;

  for (int i = 1; i <= M; i++) {
    cin >> f[i][0];
  }

  for (int j = 1; j < MAX_D; j++) {
    for (int i = 1; i <= M; i++) {
      f[i][j] = f[f[i][j - 1]][j - 1];
    }
  }

  int Q;
  cin >> Q;
  for (int i = 0; i < Q; i++) {
    int n, x;
    cin >> n >> x;

    for (int j = MAX_D - 1; j >= 0; j--) {
      if (n >= 1 << j) {
        n -= 1 << j;
        x = f[x][j];
      }
    }
    cout << x << '\n';
  }

  return 0;
}