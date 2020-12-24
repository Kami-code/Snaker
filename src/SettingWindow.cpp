#include <QSound>
#include <QSlider>
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


    slider=new QSlider(Qt::Horizontal,this);  //创建滑动条
    //Qt::Horizontal    水平滑动条       Qt::Vertical  垂直滑动条
    slider->move(300,200);
    slider->setMinimum(1); //设置最小值
    slider->setMaximum(3);  //设置最大值
    slider->setValue(1); //设置滑动条控件的值
    connect(slider,&QSlider::sliderMoved,this,&SettingWindow::SetSnakes);//用鼠标按下滑块并移动时触发信号,自动传递一个值：当前值


    slider2=new QSlider(Qt::Horizontal,this);  //创建滑动条
    //Qt::Horizontal    水平滑动条       Qt::Vertical  垂直滑动条
    slider2->move(300,250);
    slider2->setMinimum(10); //设置最小值
    slider2->setMaximum(100);  //设置最大值
    slider2->setValue(10); //设置滑动条控件的值
    connect(slider2,&QSlider::sliderMoved,this,&SettingWindow::SetBackgroundSize);//用鼠标按下滑块并移动时触发信号,自动传递一个值：当前值
}

void SettingWindow::SetSnakes(int nums) {
    game.lived_snakes = nums;
    while (nums > game.snakeList.GetCurrentLength()) {
        Snake* aa = new Snake(Point(1,1));
        game.snakeList.Insert(*aa);
    }
    while (nums < game.snakeList.GetCurrentLength()) game.snakeList.Delete();
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
    game.showAudio = !game.showAudio;
    if (game.showAudio == false) audioButton->setText("打开音效");
    else audioButton->setText("关闭音效");
    update();
}

void SettingWindow::ChangeFigureSetting()
{
    game.showFigure = !game.showFigure;
    if (game.showFigure == false) figureButton->setText("打开图形");
    else figureButton->setText("关闭图形");
    update();
}
