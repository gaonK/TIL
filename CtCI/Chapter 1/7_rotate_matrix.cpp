#include <iostream>
using namespace std;

/// time complexity: O(N^2)
/// space complexity: O(1)
void rotate(int **matrix, int N) {
    for (int i = 0; i < n / 2; i++) {
        for (int j = i; j < n - i - 1; j++) {
            int temp = matrix[i][j];
            
            matrix[i][j] = matrix[N-j-1][i];
            matrix[N-j-1][i] = matrix[N-i-1][N-j-1];
            matrix[N-i-1][N-j-1] = matrix[j][N-i-1];
            matrix[j][N-i-1] = temp;
        }
    }
}