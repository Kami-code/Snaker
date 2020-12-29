#include "header/Game.h"
#include "header/GameWindow.h"
#include "header/DesktopWindow.h"
#include "header/Saver.h"

extern Game game;
extern Saver saver;


DesktopWindow::DesktopWindow(QWidget *parent)
    : QWidget(parent), desktopSound(":/audio/audio/desktop.wav"), gameWindowSound(":/audio/audio/fight.wav")
{
    settingWindow->SetSnakes(1);
    if(game.setting.GetShowAudio())desktopSound.play();
    if (game.setting.GetShowFigure()){
        QPalette pal = this->palette();
        pal.setBrush(QPalette::Background, QBrush(QPixmap(":/image/image/title.jpg")));
        setPalette(pal);
    }
    this->setParent(parent);
    this->setWindowTitle("Game Desktop");
    this->resize(QSize(500, 500));




    InitButton();
    InitWindow();
}

DesktopWindow::~DesktopWindow()
{

}

void DesktopWindow::InitButton(){
    closeButton = new QPushButton(this);        //初始化按钮
    enterGameButton = new QPushButton(this);
    settingButton = new QPushButton(this);
    saverButton = new QPushButton(this);


    closeButton->setText("关闭窗口");           //设置按钮文字
    enterGameButton->setText("进入游戏");
    settingButton->setText("游戏设置");
    saverButton->setText("读取存档");

    closeButton->move(QPoint(50, 150));         //设置按钮标题
    enterGameButton->move(QPoint(150, 150));
    settingButton->move(QPoint(150, 350));
    saverButton->move(QPoint(150, 450));

    connect(closeButton, &QPushButton::pressed, this, &DesktopWindow::close); //点击关闭窗口后关闭
    connect(enterGameButton, &QPushButton::released, this, &DesktopWindow::ChangeToGameWindow);
    connect(settingButton, &QPushButton::released, this, &DesktopWindow::ChangeToSettingWindow);
    connect(saverButton, &QPushButton::released, this, &DesktopWindow::ChangeToSaverWindow);

}

void DesktopWindow::InitWindow() {

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

    void (DesktopWindow::*Desktop)() = &DesktopWindow::ChangeToDesktopWindow;
    void (DesktopWindow::*GameOverWindow)() = &DesktopWindow::ChangeToGameOverWindow;
    void (DesktopWindow::*SaverWindow)() = &DesktopWindow::ChangeToSaverWindow;
    void (DesktopWindow::* NoInitGameWindow)() = &DesktopWindow::ChangeToGameWindowWithNoInit;

    connect(gameWindow, funSignal2, this, GameOverWindow);
    connect(gameWindow, funSignal1, this, Desktop);
    connect(gameOverWindow, funSignal3, this, Desktop);
    connect(settingWindow, funSignal4, this, Desktop);
    connect(saverWindow, funSignal5, this, Desktop);
    connect(gameWindow, funSignal6, this, SaverWindow);
    connect(saverWindow, funSingal7, this, NoInitGameWindow);

}


void DesktopWindow::ChangeToGameWindow()
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

void DesktopWindow::ChangeToGameWindowWithNoInit()
{
    desktopSound.stop();
    if(game.setting.GetShowAudio())gameWindowSound.play();
    this->hide();
    gameWindow->show();
    gameOverWindow->hide();
    settingWindow->hide();
    saverWindow->hide();
}

void DesktopWindow::ChangeToGameOverWindow()
{
    desktopSound.stop();                                    //主菜单音乐停止
    gameWindowSound.stop();                                 //游戏音乐停止
    this->hide();                                           //隐藏界面
    gameWindow->hide();                                     //隐藏界面
    gameOverWindow->show();                                 //显示游戏结算界面
    settingWindow->hide();                                  //隐藏界面
    saverWindow->hide();                                    //隐藏界面
}

void DesktopWindow::ChangeToSettingWindow()
{
    desktopSound.stop();                                    //主菜单音乐停止
    gameWindowSound.stop();                                 //游戏音乐停止
    this->hide();                                           //隐藏界面
    gameWindow->hide();                                     //隐藏界面
    gameOverWindow->hide();                                 //隐藏界面
    settingWindow->show();                                  //显示设置界面
    saverWindow->hide();                                    //隐藏界面
}

void DesktopWindow::ChangeToSaverWindow()
{
    desktopSound.stop();                                    //主菜单音乐停止
    gameWindowSound.stop();                                 //游戏音乐停止
    this->hide();                                           //隐藏界面
    gameWindow->hide();                                     //隐藏界面
    gameOverWindow->hide();                                 //隐藏界面
    settingWindow->hide();                                  //隐藏界面
    saverWindow->show();                                    //显示存档界面
}

void DesktopWindow::ChangeToDesktopWindow() //切换到游戏主菜单
{
    gameWindowSound.stop();                                 //游戏音乐停止
    if(game.setting.GetShowAudio())desktopSound.play();     //如果音效设定打开，播放主菜单音乐
    this->show();                                           //显示主菜单
    gameWindow->hide();                                     //隐藏界面
    gameOverWindow->hide();                                 //隐藏界面
    settingWindow->hide();                                  //隐藏界面
    saverWindow->hide();                                    //隐藏界面
}
