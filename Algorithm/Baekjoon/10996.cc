#include <iostream>
using namespace std;

int main() {
  int N;
  cin >> N;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < (N - 1) / 2 + 1; j++) {
      cout << "* ";
    }
    cout << '\n';
    for (int j = 0; j < N / 2; j++) {
      cout << " *";
    }
    cout << '\n';
  }
  return 0;
}