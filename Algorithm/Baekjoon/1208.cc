#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int arr[41];
int main() {
  int N, S;
  cin >> N >> S;

  for (int i = 0; i < N; i++) {
    cin >> arr[i];
  }

  int M = N / 2;
  N = N - M;
  vector<int> v1(1 << N);
  for (int i = 0; i < (1 << N); i++) {
    for (int j = 0; j < N; j++) {
      if (i & (1 << j)) {
        v1[i] += arr[j];
      }
    }
  }

  vector<int> v2(1 << M);
  for (int i = 0; i < (1 << M); i++) {
    for (int j = 0; j < M; j++) {
      if (i & 1 << j) {
        v2[i] += arr[N + j];
      }
    }
  }

  sort(v1.begin(), v1.end());
  sort(v2.begin(), v2.end(), greater<int>());

  N = (1 << N);
  M = (1 << M);
  int i = 0;
  int j = 0;
  long long ans = 0;
  while (i < N && j < M) {
    if (v1[i] + v2[j] == S) {
      long long c1 = 1;
      long long c2 = 1;
      i++;
      j++;
      while (i < N && v1[i] == v1[i - 1]) {
        c1++;
        i++;
      }
      while (j < M && v2[j] == v2[j - 1]) {
        c2++;
        j++;
      }
      ans += c1 * c2;
    } else if (v1[i] + v2[j] < S) {
      i++;
    } else {
      j++;
    }
  }

  if (S == 0) ans--;
  cout << ans << '\n';

  return 0;
}