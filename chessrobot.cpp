#include "chessrobot.h"

chessRobot::chessRobot(GameBoard *game)
{
    this->game = game;
    initBoard();
}

chessRobot::~chessRobot(){
}

void chessRobot::initBoard(){
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            board[r][c] = game->getCell(r, c);
}

bool chessRobot::dropToken(int col, int &rowOut, char player) {
    for (int row = ROWS - 1; row >= 0; --row) {
        if (board[row][col] == ' ') {
            board[row][col] = player;
            rowOut = row;
            return true;
        }
    }
    return false;
}

void chessRobot::eraseToken(int col){
    for (int row = 0; row < ROWS; row++) {
        if (board[row][col] != ' ') {
            board[row][col] = ' ';
            return;
        }
    }
}

bool chessRobot::checkLegal(int col){
    for (int r = 0; r < ROWS; r++)
        if (board[r][col] == ' ')
            return true;
    return false;
}

bool chessRobot::checkWin(int row, int col) {
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

bool chessRobot::checkDraw(){
    for (int c = 0; c < COLS; c++){
        if (board[0][c] == ' ')
            return false;
    }
    return true;
}

int chessRobot::evaluateBoard(bool playerOneTurn){
    int score = 0;
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++){
            score += getScore(r, c, playerOneTurn);
        }
    return score;
}

int chessRobot::getScore(int row, int col, bool playerOneTurn){
    char self = playerOneTurn ? 'X' : 'O';
    char opponent = playerOneTurn ? 'O' : 'X';
    int score = 0;
    const int dx[4] = {0, 1, 1, -1}; // → ↓ ↘ ↗
    const int dy[4] = {1, 0, 1, 1};
    for (int d = 0; d < 4; d++) {
        // check if legal
        int ni = row + 3 * dx[d];
        int nj = col + 3 * dy[d];
        if (ni >= 0 && ni < ROWS && nj >= 0 && nj < COLS) {
            int selfCount = 0, oppCount = 0, emptyCount = 0;

            for (int k = 0; k < 4; k++) {
                char c = board[row + k * dx[d]][col + k * dy[d]];
                if (c == self) selfCount++;
                else if (c == opponent) oppCount++;
                else emptyCount++;
            }

            // put to weight
            if (selfCount > 0 && oppCount == 0) {
                score += weight(selfCount);
            } else if (oppCount > 0 && selfCount == 0) {
                score -= weight(oppCount) * 2;
            }
        }
    }
    return score;
}

int chessRobot::weight(int count){
    switch (count) {
        case 4: return 30000;
        case 3: return 1000;
        case 2: return 100;
        case 1: return 10;
        default: return 0;
    }
}

int chessRobot::recursiveBot(int col, int depth, bool playerOneTurn, int brother){
    char c;
    int a, b, k, rowBack=0;

    if (playerOneTurn)
        c = (depth%2 == 0) ? 'O' : 'X';
    else
        c = (depth%2 == 0) ? 'X' : 'O';

    dropToken(col, rowBack, c);//make a virtual move to see what happens

    if (checkWin(rowBack, col)){
        if (depth%2 == 0){
            eraseToken(col);
            return -100000;
        }
        else{
            eraseToken(col);
            return 100000;
        }
    }

    if (checkDraw()){
        k = evaluateBoard(playerOneTurn);
        eraseToken(col);
        return k;
    }

    if (depth != 0){
        if(depth%2 != 0){
            a = 1000000;
            for (int i = 0; i < COLS; i++){
                if (!checkLegal(i)) continue;
                k = recursiveBot(i, depth - 1, playerOneTurn, a);
                if (k < brother){
                    eraseToken(col);
                    return k;
                }
                else if (a > k)
                    a = k;
            }
            eraseToken(col);
            return a;
        }
        else{
            b = -1000000;
            for (int i = 0; i < COLS; i++){
                if (!checkLegal(i)) continue;
                k = recursiveBot(i, depth - 1, playerOneTurn, b);
                if (k > brother){
                    eraseToken(col);
                    return k;
                }
                else if (b < k)
                    b = k;
                }
            eraseToken(col);
            return b;
        }
    }
    else{
        k = evaluateBoard(playerOneTurn);
        eraseToken(col);
        return k;
    }
}

int chessRobot::ifWin(bool playerOneTurn){
    int rowBack = 0;
    for (int c = 0; c < COLS; c++){
        if (!checkLegal(c))
            continue;
        dropToken(c, rowBack, playerOneTurn ? 'X':'O');
        if (checkWin(rowBack, c)){
            eraseToken(c);
            return c;
        }
        eraseToken(c);
    }
    return 7;
}

int chessRobot::robotMoves(bool playerOneTurn, int depth){
    int b = -1000001; int robotMove = 0; int temp = 0;
    initBoard();
    robotMove = ifWin(playerOneTurn);
    if (robotMove != 7)
        return robotMove;
    robotMove = 0;
    for (int c = 0; c < COLS; c++){
        if(!checkLegal(c))
            continue;
        temp = recursiveBot(c, depth, playerOneTurn, b);
        if (temp > b){
            b = temp;
            robotMove = c;
        }
    }
    return robotMove;
}
