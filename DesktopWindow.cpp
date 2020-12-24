#include "DesktopWindow.h"
#include "GameWindow.h"
#include "Game.h"
#include <QSound>
#include "Saver.h"

extern Game game;
extern Saver saver;


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), desktopSound(":/audio/audio/desktop.wav"), gameWindowSound(":/audio/audio/fight.wav")
{
    if(game.showAudio)desktopSound.play();
    if (game.showFigure){
        QPalette pal = this->palette();
        pal.setBrush(QPalette::Background, QBrush(QPixmap(":/image/image/title.jpg")));
        setPalette(pal);
    }
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
    connect(bt3, &QPushButton::released, this, &MainWidget::changeToGameWindow);



    bt4 = new QPushButton(this);
    bt4->setText("游戏设置");
    bt4->move(QPoint(150, 350));
    connect(bt4, &QPushButton::released, this, &MainWidget::changeWindowToSettingWindow);

    bt5 = new QPushButton(this);
    bt5->setText("读取存档");
    bt5->move(QPoint(150, 450));
    connect(bt5, &QPushButton::released, this, &MainWidget::changeWindowToSaverWindow);

    gameWindow = new GameWindow();
    gameOverWindow = new GameOverWindow();
    settingWindow = new SettingWindow();
    saverWindow = new SaverWindow();

    void (GameWindow::*funSignal1)() = &GameWindow::SignalChangeToDesktop;
    void (GameWindow::*funSignal2)() = &GameWindow::SignalChangeToGameOverWindow;
    void (GameOverWindow::*funSignal3)() = &GameOverWindow::SignalChangeToDesktop;
    void (SettingWindow::*funSignal4)() = &SettingWindow::SignalChangeToDesktop;
    void (SaverWindow::*funSignal5)() = &SaverWindow::SignalChangeToDesktop;
    void (GameWindow::*funSignal6)() = &GameWindow::SignalChangeToSaverWindow;
    void (SaverWindow::*funSingal7)() = &SaverWindow::SignalChangeToGameWindow;

    void (MainWidget::*Desktop)() = &MainWidget::changeWindowToDesktop;
    void (MainWidget::*GameOverWindow)() = &MainWidget::changeToGameOverWindow;
    void (MainWidget::*SaverWindow)() = &MainWidget::changeWindowToSaverWindow;
    void (MainWidget::* NoInitGameWindow)() = &MainWidget::changeToGameWindowWithNoInit;

    connect(gameWindow, funSignal2, this, GameOverWindow);
    connect(gameWindow, funSignal1, this, Desktop);
    connect(gameOverWindow, funSignal3, this, Desktop);
    connect(settingWindow, funSignal4, this, Desktop);
    connect(saverWindow, funSignal5, this, Desktop);
    connect(gameWindow, funSignal6, this, SaverWindow);
    connect(saverWindow, funSingal7, this, NoInitGameWindow);
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



void MainWidget::changeToGameWindow()
{
    desktopSound.stop();
    if(game.showAudio)gameWindowSound.play();
    game.ReInit();
    this->hide();
    gameWindow->show();
    gameOverWindow->hide();
    settingWindow->hide();
    saverWindow->hide();
}

void MainWidget::changeToGameWindowWithNoInit()
{
    desktopSound.stop();
    if(game.showAudio)gameWindowSound.play();
    this->hide();
    gameWindow->show();
    gameOverWindow->hide();
    settingWindow->hide();
    saverWindow->hide();
}

void MainWidget::changeToGameOverWindow()
{
    desktopSound.stop();
    gameWindowSound.stop();
    this->hide();
    gameWindow->hide();
    gameOverWindow->show();
    settingWindow->hide();
    saverWindow->hide();
}

void MainWidget::changeWindowToSettingWindow()
{
    desktopSound.stop();
    gameWindowSound.stop();
    this->hide();
    gameWindow->hide();
    gameOverWindow->hide();
    settingWindow->show();
    saverWindow->hide();
}

void MainWidget::changeWindowToSaverWindow()
{
    desktopSound.stop();
    gameWindowSound.stop();
    this->hide();
    gameWindow->hide();
    gameOverWindow->hide();
    settingWindow->hide();
    saverWindow->show();
}

void MainWidget::changeWindowToDesktop()
{
    gameWindowSound.stop();
    if(game.showAudio)desktopSound.play();
    this->show();
    gameWindow->hide();
    gameOverWindow->hide();
    settingWindow->hide();
    saverWindow->hide();
}
