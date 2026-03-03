#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include "gameboard.h"
#include "chessrobot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int defaltDepth;
    bool ifRobot;

private slots:
    void handleColumnClick(int col);
    void handleBotClick(bool playerOneTurn);
    void handleRegretClick();

private:
    Ui::MainWindow *ui;
    bool playerOneTurn;
    int rowBack, colBack;
    GameBoard *game;
    chessRobot *bot;
    void initBoard();
    void updateUI();
    void dropToken(int col);
    void restart();
};

#endif // MAINWINDOW_H
