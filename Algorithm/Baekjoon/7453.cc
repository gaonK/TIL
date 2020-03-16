// https://acmicpc.net/problem/7453
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
  cin.tie(NULL);
  ios::sync_with_stdio(false);
  
  int N;
  cin >> N;

  vector<int> A(N), B(N), C(N), D(N);

  for (int i = 0; i < N; i++) {
    cin >> A[i] >> B[i] >> C[i] >> D[i];
  }

  vector<int> v1;
  vector<int> v2;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      v1.push_back(A[i] + B[j]);
      v2.push_back(C[i] + D[j]);
    }
  }

  sort(v1.begin(), v1.end());
  sort(v2.begin(), v2.end(), greater<int>());

  int i = 0;
  int j = 0;
  long long ans = 0;
  while (i < N * N && j < N * N) {
    if (v1[i] + v2[j] == 0) {
      long long c1 = 1;
      long long c2 = 1;
      i++;
      j++;
      while (i < N * N && v1[i] == v1[i - 1]) {
        c1++;
        i++;
      }
      while (j < N * N && v2[j] == v2[j - 1]) {
        c2++;
        j++;
      }
      ans += c1 * c2;
    } else if (v1[i] + v2[j] < 0) {
      i++;
    } else {
      j++;
    }
  }

  cout << ans << '\n';

  return 0;
}