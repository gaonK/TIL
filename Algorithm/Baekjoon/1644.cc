// https://acmicpc.net/problem/1644
#include <iostream>
#include <vector>
using namespace std;

bool sieve[4000001];
int main() {
  int N;
  cin >> N;

  if (N == 1) {
    cout << 0 << '\n';
    return 0;
  }

  for (int i = 2; i * i <= N; i++) {
    if (sieve[i] == false) {
      for (int j = i + i; j <= N; j += i) {
        sieve[j] = true;
      }
    }
  }

  vector<int> v;
  for (int i = 2; i <= N; i++) {
    if (!sieve[i]) {
      v.push_back(i);
    }
  }

  int size = v.size();
  int i = 0;
  int j = 0;
  int sum = v[0];
  int ans = 0;

  while (i <= j && j < size) {
    if (sum < N) {
      j++;
      sum += v[j];
    } else if (sum == N) {
      sum -= v[i];
      i++;
      ans += 1;
    } else if (sum > N) {
      sum -= v[i];
      i++;
    }
  }

  cout << ans << '\n';

  return 0;
}