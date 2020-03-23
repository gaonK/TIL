// https://acmicpc.net/problem/10868
#include <iostream>
#include <algorithm>
using namespace std;

long long arr[100001];
long long tree[1000000];

void init(int node, int st, int ed) {
  if (st == ed) {
    tree[node] = arr[st];
    return;
  }
  init(node * 2, st, (st + ed) / 2);
  init(node * 2 + 1, (st + ed) / 2 + 1, ed);
  tree[node] = min(tree[node * 2], tree[node * 2 + 1]);
}

long long query(int node, int st, int ed, int i, int j) {
  if (i > ed || j < st) {
    return -1;
  }
  if (i <= st && ed <= j) {
    return tree[node];
  }
  int m1 = query(2 * node, st, (st + ed) / 2, i, j);
  int m2 = query(2 * node + 1, (st + ed) / 2 + 1, ed, i, j);
  if (m1 == -1) return m2;
  else if (m2 == -1) return m1;
  else return min(m1, m2);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int N, M;
  cin >> N >> M;

  for (int i = 1; i <= N; i++) {
    cin >> arr[i];
  }

  init(1, 1, N);
  for (int i = 0; i < M; i++) {
    int a, b;
    cin >> a >> b;
    cout << query(1, 1, N, a, b) << '\n';
  }
  
  return 0;
}