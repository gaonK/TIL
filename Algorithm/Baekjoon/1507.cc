// https://acmicpc.net/problem/1507
#include <iostream>
using namespace std;

int d[21][21];

int main() {
  int N;
  cin >> N;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cin >> d[i][j];
    }
  }

  int answer = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j) continue;
      bool flag = false;
      for (int k = 0; k < N; k++) {
        if (i == k || k == j) continue;
        if (d[i][k] + d[k][j] < d[i][j]) {
          cout << -1 << '\n';
          return 0;
        }
        if (d[i][k] + d[k][j] == d[i][j]) {
          flag = true;
        }
      }
      if (!flag) answer += d[i][j];
    }
  }
  
  cout << answer / 2 << '\n';

  return 0;
}