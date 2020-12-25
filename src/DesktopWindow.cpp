#include <QSound>
#include "header/Game.h"
#include "header/GameWindow.h"
#include "header/DesktopWindow.h"
#include "header/Saver.h"

extern Game game;
extern Saver saver;


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), desktopSound(":/audio/audio/desktop.wav"), gameWindowSound(":/audio/audio/fight.wav")
{
    if(game.setting.GetShowAudio())desktopSound.play();
    if (game.setting.GetShowFigure()){
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
    connect(bt1, &QPushButton::pressed, this, &MainWidget::close); //点击关闭窗口后关闭


    bt3 = new QPushButton(this);
    bt3->setText("进入游戏");
    bt3->move(QPoint(150, 150));
    connect(bt3, &QPushButton::released, this, &MainWidget::ChangeToGameWindow);



    bt4 = new QPushButton(this);
    bt4->setText("游戏设置");
    bt4->move(QPoint(150, 350));
    connect(bt4, &QPushButton::released, this, &MainWidget::ChangeToSettingWindow);

    bt5 = new QPushButton(this);
    bt5->setText("读取存档");
    bt5->move(QPoint(150, 450));
    connect(bt5, &QPushButton::released, this, &MainWidget::ChangeToSaverWindow);

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

    void (MainWidget::*Desktop)() = &MainWidget::ChangeToDesktopWindow;
    void (MainWidget::*GameOverWindow)() = &MainWidget::ChangeToGameOverWindow;
    void (MainWidget::*SaverWindow)() = &MainWidget::ChangeToSaverWindow;
    void (MainWidget::* NoInitGameWindow)() = &MainWidget::ChangeToGameWindowWithNoInit;

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

}


void MainWidget::ChangeToGameWindow()
{
    desktopSound.stop();
    if(game.setting.GetShowAudio())gameWindowSound.play();
    game.ReInit();
    this->hide();
    gameWindow->show();
    gameOverWindow->hide();
    settingWindow->hide();
    saverWindow->hide();
}

void MainWidget::ChangeToGameWindowWithNoInit()
{
    desktopSound.stop();
    if(game.setting.GetShowAudio())gameWindowSound.play();
    this->hide();
    gameWindow->show();
    gameOverWindow->hide();
    settingWindow->hide();
    saverWindow->hide();
}

void MainWidget::ChangeToGameOverWindow()
{
    desktopSound.stop();
    gameWindowSound.stop();
    this->hide();
    gameWindow->hide();
    gameOverWindow->show();
    settingWindow->hide();
    saverWindow->hide();
}

void MainWidget::ChangeToSettingWindow()
{
    desktopSound.stop();
    gameWindowSound.stop();
    this->hide();
    gameWindow->hide();
    gameOverWindow->hide();
    settingWindow->show();
    saverWindow->hide();
}

void MainWidget::ChangeToSaverWindow()
{
    desktopSound.stop();
    gameWindowSound.stop();
    this->hide();
    gameWindow->hide();
    gameOverWindow->hide();
    settingWindow->hide();
    saverWindow->show();
}

void MainWidget::ChangeToDesktopWindow()
{
    gameWindowSound.stop();
    if(game.setting.GetShowAudio())desktopSound.play();
    this->show();
    gameWindow->hide();
    gameOverWindow->hide();
    settingWindow->hide();
    saverWindow->hide();
}
