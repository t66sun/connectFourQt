#ifndef CHESSROBOT_H
#define CHESSROBOT_H

#include <gameboard.h>


class chessRobot
{
public:
    chessRobot(GameBoard *game);
    ~chessRobot();
    void initBoard();
    bool dropToken(int col, int &rowOut, char player);
    void eraseToken(int col);//
    bool checkWin(int lastRow, int lastCol);
    bool checkDraw();
    bool checkLegal(int col);//
    int evaluateBoard(bool playerOneTurn);
    int getScore(int row, int col, bool playerOneTurn);
    int weight (int count);
    int recursiveBot(int col, int depth, bool playerOneTurn, int brother);
    int robotMoves (bool playerOneTurn, int depth);
    int ifWin(bool playerOneTurn);

private:
    char board[ROWS][COLS];
    GameBoard *game;
};


#endif // CHESSROBOT_H
