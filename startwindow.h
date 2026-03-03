#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class startwindow;
}

class startwindow : public QWidget
{
    Q_OBJECT

private slots:
    void startMenu();
    void setDifficulty(int input);
    void setIfRobot(bool input);

public:
    explicit startwindow(QWidget *parent = nullptr);
    ~startwindow();

private:
    Ui::startwindow *ui;
    MainWindow *w;
};

#endif // STARTWINDOW_H
