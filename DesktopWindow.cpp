#include "DesktopWindow.h"
#include "GameWindow.h"
#include "game.h"
#include <QSound>

extern Game game;


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), desktopSound(":/sound/desktop.wav"), gameWindowSound(":/sound/fight.wav")
{
    if(game.showAudio)desktopSound.play();
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QPixmap(":/image/title.jpg")));
    setPalette(pal);

    this->setParent(parent);
    this->setWindowTitle("Game Desktop");
    this->resize(QSize(500, 500));

    bt1 = new QPushButton(this);
    bt1->setText("关闭窗口");
    bt1->move(QPoint(50, 50));

    /**
     * bt1: 信号发出者, 指针类型
     * &QPushButton::pressed: 信号的发射  &发送者的类名::信号名字
     * this: 信号接受者, 指针类型
     * &MainWidget::close: 信号的处理  &接收的类名::槽名字
     *
     */
    connect(bt1, &QPushButton::pressed, this, &MainWidget::close); //点击关闭窗口后关闭

    bt2 = new QPushButton(this);
    bt2->setText("隐藏关闭窗口按钮以及改变自身文本内容");
    bt2->move(QPoint(100, 100));

    /**
     * Qt5: 槽和普通的C++成员函数几乎是一样的
     * 可以是虚函数，可以被重载，可以是public slots、protected slots、private slots，可以被其他C++成员函数直接调用
     * 唯一不同的是：槽还可以和信号连接在一起，在这种情况下，信号被发射时，会自动调用这个槽
     * 槽不需要信号传过来的参数时，可以不要参数；但槽函数一旦要参数，其参数个数，类型，顺序必须要和对应的信号保持一致。另外，槽函数的参数不能有缺省值。
     * 由于信号的返回值为void, 槽必须和信号的返回值保持一致, 因此槽的返回值为void
     *
     */
    connect(bt2, &QPushButton::released, this, &MainWidget::myButtonReleasedSlot); //按下后对“自己”操作myButtonReleasedSlot函数
    connect(bt2, &QPushButton::released, bt1, &QPushButton::hide); //按下后对“bt1”按钮操作hide函数

    bt3 = new QPushButton(this);
    bt3->setText("进入游戏");
    bt3->move(QPoint(150, 150));
    connect(bt3, &QPushButton::released, this, &MainWidget::changeWindowToSw1);



    bt4 = new QPushButton(this);
    bt4->setText("游戏设置");
    bt4->move(QPoint(500, 150));
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
    /**
     * Lambda 表达式, 匿名函数对象
     * C++11 新增加的特性, 编译时需要添加参数C++11
     * Qt配合信号一起使用, 非常方便
     */
    QPushButton *bt5 = new QPushButton(this);
    bt5->setText("Lambda表达式并统计次数");
    bt5->move(QPoint(200, 200));
    int cout = 0;
    connect(bt5, &QPushButton::clicked, this,
        // = : 把外部所有局部变量、类中所有成员以值传递方式
        // this : 类中所有成员以值传递方式
        // & : 把外部所有局部变量, 引用符号
        [=](bool isCheck) mutable {
            cout++;
            bt5->setText("点击次数: " + QString::number(cout));
//            qDebug() << "这是一个Lambda表达式测试输出" <<endl;
            //qDebug() << "打印参数:" << isCheck <<endl;
        });
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
