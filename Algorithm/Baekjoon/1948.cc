// https://acmicpc.net/problem/1948
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#define pii pair<int, int>

int dist[10001];
int dist2[10001];
int ck[10001];
int answer = 0;
vector<vector<pii> > v(10001);

void dfs(int st) {
  for (pii p : v[st]) {
    if (!dist[p.first]) {
      dfs(p.first);
    }
    if (dist[p.first] + p.second > dist[st]) dist[st] = dist[p.first] + p.second;
  }
}

void dfs2(int st) {
  if (!ck[st]) {
    ck[st] = 1;
    for (pii p : v[st]) {
      if (dist[p.first] + p.second == dist[st]) {
        dfs2(p.first);
        answer += 1;
      }
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int N, M;
  cin >> N >> M;

  for (int i = 0; i < M; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    v[a].push_back(pii(b, c));
  }

  int st, ed;
  cin >> st >> ed;
  dfs(st);
  dfs2(st);
  cout << dist[st] << '\n';
  cout << answer << '\n';

  return 0;
}
