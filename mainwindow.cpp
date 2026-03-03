#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), playerOneTurn(true), rowBack(0), colBack(0), game(new GameBoard), bot(new chessRobot(game))
{
    ui->setupUi(this);
    initBoard();
    QVBoxLayout *panel = new QVBoxLayout(ui->centralwidget);
    QGridLayout *grid = new QGridLayout();
    panel->addLayout(grid);
    grid->setSpacing(2);
    for (int col = 0; col < COLS; col++) {
        QPushButton *btn = new QPushButton(QString::number(col + 1));
        btn->setMinimumHeight(75);
        connect(btn, &QPushButton::clicked, [this, col]() {
            handleColumnClick(col);
        });
        grid->addWidget(btn, 0, col);
    }

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            QWidget *cell = new QWidget(this);
            cell->setMinimumSize(75, 100);
            cell->setStyleSheet("background-color: white; border: 1px solid black;");
            grid->addWidget(cell, row + 1, col);
        }
    }
    QPushButton *botBtn = new QPushButton("Bot Moves");
    botBtn->setMinimumHeight(75);
    connect(botBtn, &QPushButton::clicked, this, &MainWindow::handleBotClick);
    panel->addWidget(botBtn);
    QPushButton *rgtBtn = new QPushButton("Undo");
    rgtBtn->setMinimumHeight(75);
    connect(rgtBtn, &QPushButton::clicked, this, &MainWindow::handleRegretClick);
    panel->addWidget(rgtBtn);
    QPushButton *rstBtn = new QPushButton("Restart");
    rstBtn->setMinimumHeight(75);
    connect(rstBtn, &QPushButton::clicked, this, &MainWindow::restart);
    panel->addWidget(rstBtn);
    updateUI();
}

MainWindow::~MainWindow()
{
    delete bot;
    delete game;
    delete ui;
}

void MainWindow::initBoard()
{
    game->reset();
}

void MainWindow::handleColumnClick(int col)
{
    dropToken(col);
    updateUI();
    if (ifRobot){
        MainWindow::handleBotClick(playerOneTurn);
    }
}

void MainWindow::handleBotClick(bool playerOneTurn){
    int move = bot->robotMoves(playerOneTurn, defaltDepth*2-1);
    dropToken(move);
    updateUI();
}

void MainWindow::handleRegretClick(){
    if (!game->deleteToken(rowBack, colBack))
        QMessageBox::information(this, "Warning!", "You can only withdraw once!");
    playerOneTurn = !playerOneTurn;
    updateUI();
}

void MainWindow::dropToken(int col)
{
    if (game->dropToken(col, rowBack, playerOneTurn ? 'X' : 'O')){
        colBack = col;
        if (game->checkWin(rowBack, col)){
            if(playerOneTurn)
                QMessageBox::information(this, "Game Over!", "Player 1 wins!");
            else
                QMessageBox::information(this, "Game Over!", "Player 2 wins!");
        }
        if (game->checkDraw())
            QMessageBox::information(this, "Game Over!", "It's a Draw!");
        playerOneTurn = !playerOneTurn;
        return;
    }
    QMessageBox::information(this, "Warning", "Full Column!");
}

void MainWindow::updateUI()
{
    QVBoxLayout *panel = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());
    QGridLayout *grid = qobject_cast<QGridLayout*>(panel->itemAt(0)->layout());
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            QWidget *cell = qobject_cast<QWidget*>(grid->itemAtPosition(row + 1, col)->widget());
            if (game->getCell(row, col) == 'X') {
                cell->setStyleSheet("background-color: red; border: 1px solid black;");
            } else if (game->getCell(row, col) == 'O') {
                cell->setStyleSheet("background-color: yellow; border: 1px solid black;");
            } else {
                cell->setStyleSheet("background-color: white; border: 1px solid black;");
            }
        }
    }
}

void MainWindow::restart(){
    initBoard();
    playerOneTurn = true;
    updateUI();
}
