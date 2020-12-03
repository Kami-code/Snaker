#include "DesktopWindow.h"
#include "GameWindow.h"
#include "Game.h"
#include <QSound>

extern Game game;


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), desktopSound(":/audio/audio/desktop.wav"), gameWindowSound(":/audio/audio/fight.wav")
{
    if(game.showAudio)desktopSound.play();
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QPixmap(":/image/image/title.jpg")));
    setPalette(pal);

    this->setParent(parent);
    this->setWindowTitle("Game Desktop");
    this->resize(QSize(500, 500));

    bt1 = new QPushButton(this);
    bt1->setText("关闭窗口");
    bt1->move(QPoint(50, 150));

    /**
     * bt1: 信号发出者, 指针类型
     * &QPushButton::pressed: 信号的发射  &发送者的类名::信号名字
     * this: 信号接受者, 指针类型
     * &MainWidget::close: 信号的处理  &接收的类名::槽名字
     *
     */
    connect(bt1, &QPushButton::pressed, this, &MainWidget::close); //点击关闭窗口后关闭


    bt3 = new QPushButton(this);
    bt3->setText("进入游戏");
    bt3->move(QPoint(150, 150));
    connect(bt3, &QPushButton::released, this, &MainWidget::changeWindowToSw1);



    bt4 = new QPushButton(this);
    bt4->setText("游戏设置");
    bt4->move(QPoint(150, 350));
    connect(bt4, &QPushButton::released, this, &MainWidget::changeWindowToSw3);

    sw1 = new GameWindow();
    sw2 = new GameOverWindow();
    sw3 = new SettingWindow();

    void (GameWindow::*funSignal1)() = &GameWindow::SignalChangeToDesktop;
    void (MainWidget::*funSlot1)() = &MainWidget::changeWindowToDesktop;
    void (GameWindow::*funSignal2)() = &GameWindow::SignalChangeToGameOverWindow;
    void (MainWidget::*funSlot2)() = &MainWidget::changeWindowToSw2;
    void (GameOverWindow::*funSignal3)() = &GameOverWindow::SignalChangeToDesktop;
    void (SettingWindow::*funSignal4)() = &SettingWindow::SignalChangeToDesktop;
    connect(sw1, funSignal1, this, funSlot1);
    //connect(sw1, funSignalParm1, this, funSlotParm1);
    connect(sw1, funSignal2, this, funSlot2);
    connect(sw2, funSignal3, this, funSlot1);
    connect(sw3, funSignal4, this, funSlot1);
}

MainWidget::~MainWidget()
{
    // 直接或间接继承于QObject的对象, 在释放的内存的时候, 会自动调用析构函数, 释放子对象的内存
    // 故QPushButton bt1 bt2 bt3 bt4 会随着QWidget MainWidget的释放而自动调用子对象的析构函数, 释放子对象的内存
}

void MainWidget::myButtonReleasedSlot()
{
    if (bt2 != nullptr)
    {
        bt2->setText("发送信号, 槽处理完成 ...");
    }
}



void MainWidget::changeWindowToSw1()
{
    desktopSound.stop();
    if(game.showAudio)gameWindowSound.play();
    game.reInit();
    this->hide();
    sw1->show();
    sw2->hide();
    sw3->hide();
}

void MainWidget::changeWindowToSw2()
{
    desktopSound.stop();
    gameWindowSound.stop();
    this->hide();
    sw1->hide();
    sw2->show();
    sw3->hide();
}

void MainWidget::changeWindowToSw3()
{
    desktopSound.stop();
    gameWindowSound.stop();
    this->hide();
    sw1->hide();
    sw2->hide();
    sw3->show();
}

void MainWidget::changeWindowToDesktop()
{
    gameWindowSound.stop();
    if(game.showAudio)desktopSound.play();
    this->show();
    sw1->hide();
    sw2->hide();
    sw3->hide();
}

void MainWidget::myDealSubWidget1Para(int num, QString str)
{
    qDebug() << "num: " << num << " str: " << str.toUtf8().data();
    qDebug() << "num: " << num << " str: " << str.toStdString().c_str();

    qWarning() << "num: " << num << " str: " << str.toUtf8().data();
    qInfo() << "num: " << num << " str: " << str.toUtf8().data();
    //qFatal(str.toUtf8().data());
}
