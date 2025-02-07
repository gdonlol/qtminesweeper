#include "mainwindow.h"
#include "gamenotif.h"
#include <random>
#include <numeric>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //initialize grid layout
    QGridLayout *layout = new QGridLayout;

    //initialize mines matrix
    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 30; col++) {
            mines[row][col] = 0;
        }
    }

    //this is for random mine gen. basically i make an array of indexes from 0 to 480 using iota then use shuffle to mix the indexes.
    //we can then take the frist 99 numbers from this array, and plop them into the mines matrix
    std::vector<int> mine_slots(480);
    std::iota(mine_slots.begin(), mine_slots.end(), 0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(mine_slots.begin(), mine_slots.end(), gen);

    //here we place mines
    for (int i = 0; i < 99; i++) {
        mines[mine_slots[i]%16][mine_slots[i]/16] = 1;
    }

    //cheat answersheet for debug
    // for (int i = 0; i < 16; i++) {
    //     for (int j = 0; j < 30; j++) {
    //         std::cout << mines[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    //now, we create the buttons
    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 30; col++) {
            //setup button. i love how qt has css functionality, it's so easy to style in, coming from web dev
            QPushButton *button = new QPushButton(this);
            button->setStyleSheet("QPushButton { height: 33px; width: 33px; border: none; border-radius: 2px ; background: #383a40; outline: none; opacity: 1.0; font-weight: 600; color: #FFFFFF;} QPushButton::disabled {background: #2b2d31;} QPushButton::hover {background: #43454d;}");
            button->setContextMenuPolicy(Qt::CustomContextMenu); //for enabling detecting right clicks (got this method off stack overflow)
            button->setFocusPolicy(Qt::NoFocus); //for removing the ugly border, although it might hinder keyboard only users
            //for left click
            connect(button, &QPushButton::released, this,
            [button, row, col, this](){
                //stick reveal function onto each button with it's respective params
                reveal(button, row, col);
            });
            //for right click
            connect(button, &QPushButton::customContextMenuRequested, this,
            [button, this](){
                flag(button);
            });
            //add it to grid
            layout->addWidget(button, row, col);
            //store the button in this 2D array. useful for the recursive reveal.
            buttons[row][col] = button;
        }
    }
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

}

//reveal code
void MainWindow::reveal(QPushButton* e, int row, int col)
{
    //off rip, disable clicking of the tile forever
    e->setEnabled(false);
    //check if the tile is a mine.
    if(mines[row][col] == 1){
        e->setText("☠");
        GameNotif *msg = new GameNotif(this, "You died.");
        msg->setFixedSize(300, 100);
        msg->show();
        return;
    }
    //skip explored tiles, and flag the tile as explored otherwise
    if(mines[row][col] == -1){return;}else{mines[row][col]= -1;}
    //increase score to keep track of winning
    score ++;
    //check for wincon
    if(score == (480 - 99)){
        e->setText("✪");
        GameNotif *msg = new GameNotif(this, "GG! You cleared the minefield!");
        msg->setFixedSize(300, 100);
        msg->show();
        return;
    }
    //call function to count mines in proximity
    int prox_mines = get_prox_mines(row, col);
    //set the button text to the number of mines in prox. as hint for player
    e->setText(prox_mines == 0 ? " " : QString::number(prox_mines));
    //if there are no prox mines, this means we can recursively reveal surrounding non-mine tiles
    if(prox_mines == 0){
        //this loop is basically for traversing all permutations of adding or subtracting 1 to row and col, which = traverse all directions
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue; //skip self

                int prox_row = row + i;
                int prox_col = col + j;
                //check for edge cases
                if (prox_row >= 0 && prox_row < 16 && prox_col >= 0 && prox_col < 30) {
                    if (mines[prox_row][prox_col] != 1) { //if the tile isin't a mine, run reveal
                        reveal(buttons[prox_row][prox_col], prox_row, prox_col);
                    }
                }
            }
        }
    }
}
//code for counting number of mines around tile
int MainWindow::get_prox_mines(int row, int col)
{
    int prox_mines = 0;
    //this loop is basically for traversing all permutations of adding or subtracting 1 to row and col, which = traverse all directions
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;

            int prox_row = row + i;
            int prox_col = col + j;
            //check edge cases
            if (prox_row >= 0 && prox_row < 16 && prox_col >= 0 && prox_col < 30) {
                if (mines[prox_row][prox_col] == 1) {
                    prox_mines++; //on mine, inc output
                }
            }
        }
    }

    return prox_mines;
}

//simply making use of text comparison and unicode characters to display and swap icons efficiently
void MainWindow::flag(QPushButton* e){
    if(e->text() == "⚑"){
        e->setText("?");
    }
    else if(e->text() == "?"){
        e->setText(" ");
    }
    else{
        e->setText("⚑");
    }
}
