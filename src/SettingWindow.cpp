#include <QSound>
#include <QSlider>
#include <QSpinBox>
#include <QTextBrowser>
#include "header/Game.h"
#include "header/GameWindow.h"
#include "header/SettingWindow.h"

extern Game game;

SettingWindow::SettingWindow(QWidget *parent)
    : QWidget(parent)
{

    this->setParent(parent);
    this->setWindowTitle("Settings");
    this->resize(QSize(640, 480));
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QPixmap(":/image/image/title3.jpg")));
    setPalette(pal);
    audioButton = new QPushButton(this);
    audioButton->move(QPoint(300, 300));
    ChangeAudioSetting();ChangeAudioSetting(); //显示正确的文字
    connect(audioButton, &QPushButton::clicked, this, &SettingWindow::ChangeAudioSetting); //被点击的时候发射信号

    figureButton = new QPushButton(this);
    figureButton->move(QPoint(300, 380));
    ChangeFigureSetting();ChangeFigureSetting(); //显示正确的文字
    connect(figureButton, &QPushButton::clicked, this, &SettingWindow::ChangeFigureSetting); //被点击的时候发射信号

    returnButton = new QPushButton(this);
    returnButton->move(QPoint(300, 460));
    returnButton->setText("返回主界面");
    connect(returnButton, &QPushButton::clicked, this, &SettingWindow::ChangeToDesktop);

    QTextBrowser *textBrowser = new QTextBrowser(this);
    textBrowser->setText("蛇的数量");
    textBrowser->move(150,200);
    textBrowser->setStyleSheet("background:transparent;border-width:0;border-style:outset");
    QSpinBox *pSpinBox = new QSpinBox(this);
    pSpinBox->setMinimum(1);  // 最小值
    pSpinBox->setMaximum(maxSnake);  // 最大值
    pSpinBox->setSingleStep(1);  // 步长
    pSpinBox->move(250,200);
    slider=new QSlider(Qt::Horizontal,this);  //创建滑动条
    //Qt::Horizontal    水平滑动条       Qt::Vertical  垂直滑动条
    slider->move(300,200);
    slider->setMinimum(1); //设置最小值
    slider->setMaximum(maxSnake);  //设置最大值
    slider->setValue(1); //设置滑动条控件的值
    connect(slider,&QSlider::valueChanged,this,&SettingWindow::SetSnakes);//用鼠标按下滑块并移动时触发信号,自动传递一个值：当前值
    connect(slider, SIGNAL(valueChanged(int)), pSpinBox, SLOT(setValue(int)));
    connect(pSpinBox,SIGNAL(valueChanged(int)),slider,SLOT(setValue(int)));


    QTextBrowser *textBrowser2 = new QTextBrowser(this);
    textBrowser2->setText("地图大小");
    textBrowser2->move(150,250);
    textBrowser2->setStyleSheet("background:transparent;border-width:0;border-style:outset");
    QSpinBox *pSpinBox2 = new QSpinBox(this);
    pSpinBox2->setMinimum(10);  // 最小值
    pSpinBox2->setMaximum(100);  // 最大值
    pSpinBox2->setSingleStep(10);  // 步长
    pSpinBox2->move(250,250);
    slider2=new QSlider(Qt::Horizontal,this);  //创建滑动条
    //Qt::Horizontal    水平滑动条       Qt::Vertical  垂直滑动条
    slider2->move(300,250);
    slider2->setMinimum(10); //设置最小值
    slider2->setMaximum(100);  //设置最大值
    slider2->setValue(10); //设置滑动条控件的值
    connect(slider2,&QSlider::valueChanged,this,&SettingWindow::SetBackgroundSize);//用鼠标按下滑块并移动时触发信号,自动传递一个值：当前值
    connect(slider2, SIGNAL(valueChanged(int)), pSpinBox2, SLOT(setValue(int)));
    connect(pSpinBox2,SIGNAL(valueChanged(int)),slider2,SLOT(setValue(int)));
}

void SettingWindow::SetSnakes(int nums) {
    game.setting.SetLivedSnakeNumber(nums);
    int count = 1;
    while (game.snakeList.GetCurrentLength() != 0) game.snakeList.Delete();
    while (game.snakeList.GetCurrentLength() != nums) {
        Point headPoint = game.background.GetRandomVaildPoint();
        Snake* aa = new Snake(headPoint);
        game.background.GetGround()[headPoint.x][headPoint.y].y = 1;
        game.snakeList.Insert(*aa);
        count++;
    }
}


void SettingWindow::SetBackgroundSize(int nums) {
    game.background.ClearGround();
    game.background.SetHeight(nums);
    game.background.SetWidth(nums);
    game.background.SetGround();
}

SettingWindow::~SettingWindow()
{
}

void SettingWindow::ChangeToDesktop()
{
    emit SignalChangeToDesktop();
}

void SettingWindow::ChangeAudioSetting()
{
    game.setting.SetShowAudio(!game.setting.GetShowAudio());
    if (game.setting.GetShowAudio() == false) audioButton->setText("打开音效");
    else audioButton->setText("关闭音效");
    update();
}

void SettingWindow::ChangeFigureSetting()
{
    game.setting.SetShowFigure(!game.setting.GetShowFigure());
    if (game.setting.GetShowFigure() == false) figureButton->setText("打开图形");
    else figureButton->setText("关闭图形");
    update();
}
