#ifndef GAMENOTIF_H
#define GAMENOTIF_H

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProcess>

class GameNotif : public QDialog
{
    Q_OBJECT

public:
    explicit GameNotif(QWidget *parent = nullptr, const QString &message = "");

private:
    QLabel *label;

};

#endif // GAMENOTIF_H
