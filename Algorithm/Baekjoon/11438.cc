// https://acmicpc.net/problem/11438
// (reference) https://blog.naver.com/kks227/220820773477
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
#define MAX 18

int p[100001][MAX];
int main() {
  cin.tie(NULL);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;

  vector<vector<int> > v(N + 1);
  for (int i = 0; i < N - 1; i++) {
    int a, b;
    cin >> a >> b;
    v[a].push_back(b);
    v[b].push_back(a);
  }

  queue<int> q;
  vector<bool> visited(N + 1);
  vector<int> level(N + 1);
  q.push(1);
  level[1] = 1;
  visited[1] = true;

  // Find parent of node (bfs)
  while (!q.empty()) {
    int x = q.front();
    q.pop();

    for (int nx: v[x]) {
      if (!visited[nx]) {
        visited[nx] = true;
        p[nx][0] = x;
        level[nx] = level[x] + 1;
        q.push(nx);
      }
    }
  }
  
  for (int j = 0; (1 << j) < N; j++) {
    for (int i = 1; i <= N; i++) {
      if (p[i][j] != 0) {
        p[i][j + 1] = p[p[i][j]][j];
      }
    }
  }

  int M;
  cin >> M;
  for (int i = 0; i < M; i++) {
    int a, b;
    cin >> a >> b;
    if (level[a] < level[b]) {
      swap(a, b);
    }
    int diff = level[a] - level[b];

    for (int j = 0; diff; j++) {
      if (diff % 2) a = p[a][j];
      diff /= 2;
    }

    if (a != b) {
      for (int j = MAX - 1; j >= 0; j--) {
        if (p[a][j] != 0 && p[a][j] != p[b][j]) {
          a = p[a][j];
          b = p[b][j];
        }
      }
      a = p[a][0];
    }

    cout << a << '\n';
  }

  return 0;
}