// https://acmicpc.net/problem/3176
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#define pii pair<int, int>
using namespace std;

int p[100001][22];
int maxDist[100001][22];
int minDist[100001][22];

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int N;
  cin >> N;

  vector<vector<pii> > v(N + 1);
  for (int i = 0; i < N - 1; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    v[a].push_back(pii(b, c));
    v[b].push_back(pii(a, c));
  }

  queue<int> q;
  vector<bool> visited(N + 1);
  vector<int> depth(N + 1);
  q.push(1);
  visited[1] = true;
  while (!q.empty()) {
    int x = q.front();
    q.pop();

    for (auto next: v[x]) {
      int nx = next.first;
      if (!visited[nx]) {
        visited[nx] = true;
        depth[nx] = depth[x] + 1;
        p[nx][0] = x;
        int cost = next.second;
        minDist[nx][0] = cost;
        maxDist[nx][0] = cost;
        q.push(nx);
      }
    }
  }

  for (int j = 1; j < 22; j++) {
    for (int i = 1; i <= N; i++) {
      p[i][j] = p[p[i][j - 1]][j - 1];
      minDist[i][j] = min(minDist[i][j - 1], minDist[p[i][j - 1]][j - 1]);
      maxDist[i][j] = max(maxDist[i][j - 1], maxDist[p[i][j - 1]][j - 1]);
    }
  }

  int K;
  cin >> K;
  for (int i = 0; i < K; i++) {
    int d, e;
    cin >> d >> e;

    if (depth[d] < depth[e]) {
      swap(d, e);
    }

    int minAns = 2100000000;
    int maxAns = -1;

    int diff = depth[d] - depth[e];
    for (int j = 0; diff; j++) {
      if (diff & 1) {
        minAns = min(minAns, minDist[d][j]);
        maxAns = max(maxAns, maxDist[d][j]);
        d = p[d][j];
      }
      diff >>= 1;
    }   

    if (d != e) {
      for (int j = 21; j >= 0; j--) {
        if (p[d][j] != p[e][j]) {
          minAns = min(minAns, min(minDist[d][j], minDist[e][j]));
          maxAns = max(maxAns, max(maxDist[d][j], maxDist[e][j]));
          d = p[d][j];
          e = p[e][j];
        }
      }
      minAns = min(minAns, min(minDist[d][0], minDist[e][0]));
      maxAns = max(maxAns, max(maxDist[d][0], maxDist[e][0]));
    }

    cout << minAns << ' ' << maxAns << '\n';
  }
  
  return 0;
}