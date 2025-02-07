#include "gamenotif.h"

GameNotif::GameNotif(QWidget *parent, const QString &message) : QDialog(parent) {
    setWindowTitle("Minesweeper");
    //set up widgets
    label = new QLabel(message, this);
    QPushButton *retry_button = new QPushButton("Play Again", this);
    QPushButton *exit_button = new QPushButton("Exit", this);
    //used a vertical box then an h box for the buttons
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);

    QHBoxLayout *buttons_div = new QHBoxLayout();
    buttons_div->addWidget(retry_button);
    buttons_div->addWidget(exit_button);

    layout->addLayout(buttons_div);

    //low code way i found to restart game
    connect(retry_button, &QPushButton::released, []() {
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    });
    //quit game
    connect(exit_button, &QPushButton::released, []() {
        QApplication::quit();
    });

    setLayout(layout);

}
