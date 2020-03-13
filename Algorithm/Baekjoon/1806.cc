#include <iostream>
using namespace std;

int a[100001];
int main() {
  int N, S;
  cin >> N >> S;

  for (int i = 0; i < N; i++) {
    cin >> a[i];
  }

  int i = 0;
  int j = 0;
  int sum = a[0];
  int ans = N + 1;
  while (i <= j && j < N) {
    if (sum < S) {
      sum += a[++j];
    } else if (sum == S) {
      if (j - i + 1 < ans) {
        ans = j - i + 1;
      }
      j += 1;
      sum += a[j];
    } else if (sum > S) {
      if (j - i + 1 < ans) {
        ans = j - i + 1;
      }
      sum -= a[i++];
    }
  }

  if (ans > N) ans = 0;
  cout << ans << '\n';

  return 0;
}