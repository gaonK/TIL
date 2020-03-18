// https://acmicpc.net/problem/1761
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
#define pii pair<int, int>

int main() {
  int N;
  cin >> N;
  vector<vector<pii > > v(N + 1);
  for (int i = 0; i < N - 1; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    v[a].push_back(pii(b, c));
    v[b].push_back(pii(a, c));
  }

  int M;
  cin >> M;

  queue<int> q;
  vector<int> dist(N + 1);
  vector<int> level(N + 1);
  vector<int> parent(N + 1);
  vector<int> visited(N + 1);
  q.push(1);
  visited[1] = true;
  level[1] = 1;

  while (!q.empty()) {
    int x = q.front();
    q.pop();

    for (auto next: v[x]) {
      int nx = next.first;
      int cost = next.second;
      if (!visited[nx]) {
        visited[nx] = true;
        dist[nx] = dist[x] + cost;
        level[nx] = level[x] + 1;
        parent[nx] = x;
        q.push(nx);
      }
    }
  }

  for (int i = 0; i < M; i++) {
    int a, b;
    cin >> a >> b;
    
    int ans = 0;
    while (a != b) {
      if (level[a] == level[b]) {
        ans += dist[a] - dist[parent[a]];
        ans += dist[b] - dist[parent[b]];
        a = parent[a];
        b = parent[b];
      } else if (level[a] < level[b]) {
        ans += dist[b] - dist[parent[b]];
        b = parent[b];
      } else {
        ans += dist[a] - dist[parent[a]];
        a = parent[a];
      }
    }
    cout << ans << '\n';
  }

  return 0;
}