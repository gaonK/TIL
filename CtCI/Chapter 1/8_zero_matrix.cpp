#include <iostream>
using namespace std;

/// Helper methods
void nullifyRow(int **matrix, int N, int row) {
    for (int j = 0; j < N; j++) {
        matrix[row][j] = 0;
    }
}

void nullifyColumn(int **matrix, int M, int col) {
    for (int i = 0; i < M; i++) {
        matrix[i][col] = 0;
    }
}

/// time complexity: O(MN)
/// space complexity: O(1)
void nullifyMatrix(int **matrix, int M, int N) {
    bool firstRow = false;

    for (int i = 0; i < N; i++) {
        if (matrix[0][i] == 0) {
            firstRow = true;
            break;
        }
    }

    for (int i = 1; i < M; i++) {
        bool nullifyThisRow = false;
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == 0) {
                matrix[0][j] = 0;
                nullifyThisRow = true;
            }
        }
        if (nullifyThisRow) {
            nullifyRow(matrix, N, i);
        }
    }

    for (int j = 0; j < N; j++) {
        if (matrix[0][j] == 0) {
            nullifyColumn(matrix, M, j);
        }
    }

    if (firstRow) {
        nullifyRow(matrix, N, 0);
    }
}