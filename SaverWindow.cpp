#include "SaverWindow.h"
#include "GameWindow.h"
#include <QSound>
#include "Game.h"
#include "Saver.h"
#include <QSlider>

extern Game game;
extern Saver saver;

SaverWindow::SaverWindow(QWidget *parent)
    : QWidget(parent)
{

    this->setParent(parent);
    this->setWindowTitle("Settings");
    this->resize(QSize(640, 480));
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QPixmap(":/image/image/title4.jpg")));
    setPalette(pal);


    returnButton = new QPushButton(this);
    returnButton->move(QPoint(300, 460));
    returnButton->setText("返回游戏");
    connect(returnButton, &QPushButton::clicked, this, &SaverWindow::changeToGameWindow);

    saverbutton[0] = new QPushButton(this);
    saverbutton[0]->move(QPoint(300, 220));
    if (saver.getSaved(0) == false) saverbutton[0]->setText("保存存档0");
    else saverbutton[0]->setText("读取存档0");
    connect(saverbutton[0], &QPushButton::clicked, this, &SaverWindow::loadSaver0);

    saverbutton[1]= new QPushButton(this);
    saverbutton[1]->move(QPoint(300, 300));
    if (saver.getSaved(1) == false) saverbutton[1]->setText("保存存档1");
    else saverbutton[1]->setText("读取存档1");
    connect(saverbutton[1], &QPushButton::clicked, this, &SaverWindow::loadSaver1);

    saverbutton[2] = new QPushButton(this);
    saverbutton[2]->move(QPoint(300, 380));
    if (saver.getSaved(2) == false) saverbutton[2]->setText("保存存档2");
    else saverbutton[2]->setText("读取存档2");
    connect(saverbutton[2], &QPushButton::clicked, this, &SaverWindow::loadSaver2);
}





void SaverWindow::loadSaver0() {
    if (saver.getSaved(0) == false) {
        saver.save(0, &game);
        saver.setSaved(0, true);
    }
    else {saver.load(0, &game);}
    if (saver.getSaved(0) == false) saverbutton[0]->setText("保存存档0");
    else saverbutton[0]->setText("读取存档0");
}
void SaverWindow::loadSaver1() {
    if (saver.getSaved(1) == false) {
        saver.save(1, &game);
        saver.setSaved(1, true);
    }
    else {saver.load(1, &game);}
    if (saver.getSaved(1) == false) saverbutton[1]->setText("保存存档1");
    else saverbutton[1]->setText("读取存档1");
}
void SaverWindow::loadSaver2() {
    if (saver.getSaved(2) == false) {
        saver.save(2, &game);
        saver.setSaved(2, true);
    }
    else {saver.load(2, &game);}
    if (saver.getSaved(2) == false) saverbutton[2]->setText("保存存档2");
    else saverbutton[2]->setText("读取存档2");
}

SaverWindow::~SaverWindow()
{
}

void SaverWindow::ChangeToDesktop(){
    emit SignalChangeToDesktop();
}

void SaverWindow::changeToGameWindow(){
    emit SignalChangeToGameWindow();
}

