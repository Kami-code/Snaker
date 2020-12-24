#include <QSound>
#include "header/GameWindow.h"
#include "header/GameOverWindow.h"

GameOverWindow::GameOverWindow(QWidget *parent)
    : QWidget(parent)
{

    this->setParent(parent);
    this->setWindowTitle("gameover");
    this->resize(QSize(640, 480));
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QPixmap(":/image/image/gameover.jpg")));
    setPalette(pal);
    bt = new QPushButton(this);
    bt->move(QPoint(300, 460));

    connect(bt, &QPushButton::clicked, this, &GameOverWindow::ChangeToDesktop); //被点击的时候发射信号
}


GameOverWindow::~GameOverWindow()
{
}

void GameOverWindow::ChangeToDesktop()
{
    emit SignalChangeToDesktop();
}
