#ifndef GAMEBOARD_H
#define GAMEBOARD_H


const int ROWS = 6;
const int COLS = 7;

class GameBoard
{
public:
    GameBoard();
    ~GameBoard();

    void reset();
    bool dropToken(int col, int &rowOut, char player);
    bool deleteToken(int row, int col);
    bool checkWin(int lastRow, int lastCol);
    bool checkDraw();
    bool checkLegal(int col);
    char getCell(int row, int col) const;

private:
    char board[ROWS][COLS];
    char copyBoard[ROWS][COLS];
};

#endif // GAMEBOARD_H
