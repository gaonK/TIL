// https://acmicpc.net/problem/11437
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
  int N;
  cin >> N;

  vector<vector<int> > v(N + 1);
  for (int i = 0; i < N - 1; i++) {
    int a, b;
    cin >> a >> b;
    v[a].push_back(b);
    v[b].push_back(a);
  }

  vector<int> level(N + 1);
  queue<int> q;
  vector<int> parent(N + 1);
  vector<bool> visited(N + 1);
  q.push(1);
  level[1] = 1;
  visited[1] = true;
  while (!q.empty()) {
    int x = q.front();
    q.pop();

    for (int nx : v[x]) {
      if (!visited[nx]) {
        visited[nx] = true;
        level[nx] = level[x] + 1;
        parent[nx] = x;
        q.push(nx);
      }
    }
  }

  int M;
  cin >> M;

  while (M--) {
    int a, b;
    cin >> a >> b;

    while (a != b) {
      if (level[a] < level[b]) {
        b = parent[b];
      } else if (level[a] > level[b]) {
        a = parent[a];
      } else {
        a = parent[a];
        b = parent[b];
      }
    }
    cout << a << '\n';
  }

  return 0;
}