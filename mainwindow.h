#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
private slots:
    void reveal(QPushButton *e = nullptr, int row = 0, int col = 0);
    int get_prox_mines(int row = 0, int col = 0);
    void flag(QPushButton *e = nullptr);
private:
    int mines[16][30];
    QPushButton *buttons[16][30];
    int score = 0;
};
#endif // MAINWINDOW_H
