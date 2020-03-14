// https://acmicpc.net/problem/1261
#include <cstdio>
#include <queue>
using namespace std;

#define pii pair<int, int>

int maze[101][101];
int minNum[101][101];
int dx[4] = { 1, -1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };
int INF = 10000;

int main() {
  int N, M;
  scanf("%d %d", &N, &M);

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      scanf("%1d", &maze[i][j]);
      minNum[i][j] = INF;
    }
  }

  queue<pii > q;
  q.push(pii(0, 0));
  minNum[0][0] = 0;
  while (!q.empty()) {
    int x = q.front().first;
    int y = q.front().second;
    q.pop();

    for (int i = 0; i < 4; i++) {
      int nx = x + dx[i];
      int ny = y + dy[i];

      if (0 <= nx && nx < M && 0 <= ny && ny < N) {
        if (minNum[nx][ny] > minNum[x][y] + maze[nx][ny]) {
          minNum[nx][ny] = minNum[x][y] + maze[nx][ny];
          q.push(pii(nx, ny));
        }
      }
    }
  }

  printf("%d\n", minNum[M - 1][N - 1]);

  return 0;
}