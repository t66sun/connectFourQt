#include "gameboard.h"

GameBoard::GameBoard() {
    reset();
}

GameBoard::~GameBoard(){
    reset();
}

void GameBoard::reset() {
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j)
            board[i][j] = ' ';
}

bool GameBoard::dropToken(int col, int &rowOut, char player) {
    for (int row = ROWS - 1; row >= 0; --row) {
        if (board[row][col] == ' ') {
            board[row][col] = player;
            rowOut = row;
            return true;
        }
    }
    return false;
}

bool GameBoard::deleteToken(int row, int col){
    if (board[row][col] != ' '){
        board[row][col] = ' ';
        return true;
    }
    return false;
}

char GameBoard::getCell(int row, int col) const {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS)
        return board[row][col];
    return ' ';
}

bool GameBoard::checkWin(int row, int col) {
    char current = board[row][col];
    const int dx[4] = {0, 1, 1, 1};
    const int dy[4] = {1, 0, 1, -1};

    for (int d = 0; d < 4; d++) {
        int count = 1;
        int r = row + dx[d], c = col + dy[d];
        while (r >= 0 && r < ROWS && c >= 0 && c < COLS && board[r][c] == current) {
            count++;
            r += dx[d]; c += dy[d];
        }
        r = row - dx[d]; c = col - dy[d];
        while (r >= 0 && r < ROWS && c >= 0 && c < COLS && board[r][c] == current) {
            count++;
            r -= dx[d]; c -= dy[d];
        }
        if (count >= 4)
            return true;
    }
    return false;
}

bool GameBoard::checkDraw(){
    for (int c = 0; c < COLS; c++){
        if (checkWin(0, c))
            return false;
        if (board[0][c] == ' ')
            return false;
    }
    return true;
}

bool GameBoard::checkLegal(int col){
    for (int r = 0; r < ROWS; r++)
        if (board[r][col] == ' ')
            return true;
    return false;
}
