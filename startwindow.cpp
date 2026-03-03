#include "startwindow.h"
#include "ui_startwindow.h"

startwindow::startwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::startwindow)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    w = new MainWindow;
    connect(ui->sttBtn, &QPushButton::clicked, this, &startwindow::startMenu);
    w->defaltDepth = 3;
    connect(ui->easyBtn, &QPushButton::clicked, this, [=](){startwindow::setDifficulty(3);});
    connect(ui->medBtn, &QPushButton::clicked, this, [=](){startwindow::setDifficulty(4);});
    connect(ui->hardBtn, &QPushButton::clicked, this, [=](){startwindow::setDifficulty(5);});
    w->ifRobot = false;
    connect(ui->PvPBtn, &QPushButton::clicked, this, [=](){startwindow::setIfRobot(false);});
    connect(ui->PvCBtn, &QPushButton::clicked, this, [=](){startwindow::setIfRobot(true);});
}

startwindow::~startwindow()
{
    delete ui;
}

void startwindow::startMenu(){
    w->show();
    this->hide();
}

void startwindow::setDifficulty(int input){
    w->defaltDepth = input;
    return;
}

void startwindow::setIfRobot(bool input){
    w->ifRobot = input;
    return;
}
