#include <iostream>
using namespace std;

const int N = 9;

bool isSafe(int board[N][N], int row, int col, int number) {
    for (int x = 0; x < N; x++) {
        if (board[row][x] == number)
            return false;

        if (board[x][col] == number)
            return false;
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[startRow + i][startCol + j] == number)
                return false;
        }
    }

    return true;
}

bool solveSudoku(int board[N][N]) {
    int row = -1;
    int col = -1;
    bool empty = false;

    for (int i = 0; i < N && !empty; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0) {
                row = i;
                col = j;
                empty = true;
                break;
            }
        }
    }

    if (!empty)
        return true;

    for (int number = 1; number <= 9; number++) {
        if (isSafe(board, row, col, number)) {
            board[row][col] = number;

            if (solveSudoku(board))
                return true;

            board[row][col] = 0;
        }
    }

    return false;
}

void printBoard(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
}

int main() {
    int board[N][N] = {
        {5,3,0,0,7,0,0,0,0},
        {6,0,0,1,9,5,0,0,0},
        {0,9,8,0,0,0,0,6,0},
        {8,0,0,0,6,0,0,0,3},
        {4,0,0,8,0,3,0,0,1},
        {7,0,0,0,2,0,0,0,6},
        {0,6,0,0,0,0,2,8,0},
        {0,0,0,4,1,9,0,0,5},
        {0,0,0,0,8,0,0,7,9}
    };

    if (solveSudoku(board))
        printBoard(board);
    else
        cout << "No solution exists." << endl;

    return 0;
}
