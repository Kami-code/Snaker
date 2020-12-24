#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QSound>
#include "header/Game.h"
#include "header/GameWindow.h"
#include "header/Resource.h"
#include "header/Saver.h"
#include "header/LinkList.h"

extern Game game;
extern Saver saver;

WindowMap::WindowMap(){

}

WindowMap::WindowMap(Game* game, Point a, Point b) {
    Init(game, a, b);
}

void WindowMap::Init(Game* game, Point a, Point b) {
    gameAddr = game;
    gameWidth = (*game).background.GetWidth();
    gameHeight = (*game).background.GetHeight();
    left = a.x;
    up = a.y;
    right = b.x;
    down = b.y;
    width = abs(left - right);
    height = abs(up - down);
    unitWidth = width / gameWidth;
    unitHeight = height / gameHeight;
}

/*
 * function: draw
 * Parameters:
 *          painter – a painter bound to the instance of the GameWindow
 * Description: Draw the gamewindow interface, which contains grids, snakes, foods, and walls.
 * This function is called by GameWindow::paintEvent.
 */
void WindowMap::Draw(QPainter *painter) { /*绘制游戏界面，被paintEvent调用*/
    (*painter).setRenderHint(QPainter::Antialiasing);
    (*painter).translate(left, up);
    //painter.scale(side / 200.0, side / 200.0);
    for (int i = 0; i < gameWidth; i ++) {
        for (int j = 0; j < gameHeight; j ++) {
            int status = (*gameAddr).background.GetGround()[i][j] % 100;
            if (status == 0) {
                if (game.showFigure)
                (*painter).setPen(QColor(255,255,255,255));
                else (*painter).setPen(QColor(0,0,0,255));
                (*painter).setBrush(Qt::NoBrush);
                (*painter).drawRect(unitWidth * i,unitHeight * j,unitWidth , unitHeight);/*绘制网格*/
            }
            else {
                if (!game.showFigure) {
                    (*painter).setPen(QColor(222,222,222,128));
                    (*painter).setBrush(QColor(0,255,255,128));
                    (*painter).drawRect(unitWidth * i,unitHeight * j,unitWidth , unitHeight);
                }
                else {
                    (*painter).setPen(Qt::NoPen);
                    (*painter).setBrush(QColor(0,255,255,128));
                    (*painter).drawPixmap(unitWidth * i,unitHeight * j,unitWidth , unitHeight, resource.food[status]);
                }
            }
        }
    }
    (*painter).setPen(Qt::NoPen);
    (*painter).setBrush(QColor(255,0,0,128));

    //绘制蛇
    ListNode<Snake> *head = game.snakeList.head->next;

    int cnt = 0;
    while (head != NULL) {
        ListNode<Point> *header = (head->data).GetBody().head->next;
        if (head->data.GetLife() < 0) continue;
        while (header != NULL) {
            if (!game.showFigure) {
                (*painter).drawRect(unitWidth * header->data.x, unitHeight * header->data.y,unitWidth,unitHeight);
            }
            else {
                (*painter).drawPixmap(unitWidth * header->data.x, unitHeight * header->data.y,unitWidth , unitHeight, resource.character[cnt]);
            }
            header = header->next;
        }
        head = head->next; cnt++;
    }
}

/*
 * function: leftClicked
 * Parameters:
 *          a – point of window where the mouse click.
 * Description: Get the index of the game.ground and plus one(change the ground status).
 */
void WindowMap::LeftClicked(Point a){
    if (a.x < left || a.x > right || a.y < up || a.y > down) return;
    int rowIndex = (a.y - up) / unitHeight;
    int colIndex = (a.x - left) / unitWidth;
    qDebug() << "change [" <<rowIndex << "][" << colIndex <<"]" << Qt::endl;
    (*gameAddr).background.GetGround()[colIndex][rowIndex] ++;
}

/*
 * function: GameStopped
 * Description: Reverse the flag stopped. Stop or restart the game according to the flag.
 */
void GameWindow::GameStopped(){
    if (stopped) {
        timer[0]->start(300);
        timer[1]->start(300);
        timer[2]->start(300);
        at->setText("暂停");
     }
    else if (!stopped) {
        timer[0]->stop();
        timer[1]->stop();
        timer[2]->stop();
        at->setText("开始");
    }
    stopped = !stopped;
}

/*
 * function: GameWindow
 * Description: Create the window.
 */
GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
{

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QPixmap(":/image/image/grass.png")));
    setPalette(pal);
    this->setWindowTitle("游戏界面");
    this->resize(QSize(640, 480));

    bt = new QPushButton(this);
    bt->setText("切换到主窗口");
    bt->move(QPoint(50, 50));
    connect(bt, &QPushButton::released, this, &GameWindow::ChangeToDesktop);

    at = new QPushButton(this);
    at->setText("开始");
    at->move(QPoint(100, 50));
    connect(at, &QPushButton::released, this, &GameWindow::GameStopped);

    ct = new QPushButton(this);
    ct->setText("存档");
    ct->move(QPoint(150, 50));
    connect(ct, &QPushButton::released, this, &GameWindow::Save);

//    for (int i = 0; i < 100; i++) {
//        timer[i] = new QTimer(this);
//        timer[i]->setObjectName(QString(i));
//        connect(timer[i], &QTimer::timeout, this, QOverload<>::of(&GameWindow::timelyAccess)); //定周期调用move函数
//    }
    timer[0] = new QTimer(this);
    timer[1] = new QTimer(this);
    timer[2] = new QTimer(this);
    connect(timer[0], &QTimer::timeout, this, QOverload<>::of(&GameWindow::TimelyAccess0)); //定周期调用move函数
    connect(timer[1], &QTimer::timeout, this, QOverload<>::of(&GameWindow::TimelyAccess1)); //定周期调用move函数
    connect(timer[2], &QTimer::timeout, this, QOverload<>::of(&GameWindow::TimelyAccess2)); //定周期调用move函数
    stopped = true; //一开始蛇静止
    QTimer* timer2 = new QTimer(this); //不需要写为成员变量，自动消亡
    connect(timer2, &QTimer::timeout, this, QOverload<>::of(&GameWindow::update)); //定周期刷新界面
    timer2->start(500);

    setWindowTitle(tr("Game"));
}

/*
 * function: mousePressEvent
 * Parameters:
 *          event – QMouseEvent
 * Description: When each mouse press detected, call this function to deal with it.
 */
void GameWindow::mousePressEvent(QMouseEvent *event)
{
    //左键按下
    if (event->button() == Qt::LeftButton)
    {
        Point nowPos = Point(event->pos());
        windowMap.LeftClicked(nowPos);
        //将当前鼠标点击的数据插入到主要数据数组中
        update();        //调用绘图函数
    }
    //右键按下, 删除按下位置的元素
    else if (event->button() == Qt::RightButton)
    {
        update();       //调用绘图函数
    }
}

/*
 * function: keyPressEvent
 * Parameters:
 *          event – QKeyEvent
 * Description: When each key press detected, call this function to deal with it.
 * Using W,A,S,D to move the first snake.
 * Using I,J,K,L to move the first snake.
 * Using up,down,left,right arrow to move the other snakes.
 */
void GameWindow::keyPressEvent(QKeyEvent * event) {
    int key = event->key();
    int direction[4];
    memset(direction, 0, sizeof(direction));
    switch (key) {
    case Qt::Key_W:direction[1] = 1;break;
    case Qt::Key_A:direction[1] = 2;break;
    case Qt::Key_S:direction[1] = 3;break;
    case Qt::Key_D:direction[1] = 4;break;
    case Qt::Key_I:direction[2] = 1;break;
    case Qt::Key_J:direction[2] = 2;break;
    case Qt::Key_K:direction[2] = 3;break;
    case Qt::Key_L:direction[2] = 4;break;
    case Qt::Key_Up:direction[3] = 1;break;
    case Qt::Key_Left:direction[3] = 2;break;
    case Qt::Key_Down:direction[3] = 3;break;
    case Qt::Key_Right:direction[3] = 4;break;
    }

    ListNode<Snake> *head = game.snakeList.head->next;
    int cnt = 1;
    while (head != NULL) {
        if (direction[min(3, cnt)] != 0)(head->data).SetTryDirection(direction[min(3, cnt)]);
        else (head->data).SetTryDirection((head->data).GetTryDirection());
        head = head->next;
        cnt++;
    }

}

GameWindow::~GameWindow(){

}

void GameWindow::ChangeToDesktop()
{
    timer[0]->stop();
    timer[1]->stop();
    timer[2]->stop();
    emit SignalChangeToDesktop();
}

void GameWindow::ChangeToGameOverWindow()
{
    emit SignalChangeToGameOverWindow();
    if(game.showAudio)QSound::play(":/audio/audio/failure2.wav");
    qDebug()  << "emit gameover!";
}

void GameWindow::ChangeToSaverWindow()
{
    timer[0]->stop();
    timer[1]->stop();
    timer[2]->stop();
    emit SignalChangeToSaverWindow();
}


int onTimeOut(Snake& snakeLocal) //定时器事件触发绑定
{
    if (snakeLocal.GetLife() <= 0) return -5;
    int tryDirection = snakeLocal.GetTryDirection();
    int afterstatus = 0;
    if (tryDirection == 1) {
        afterstatus = game.SnakeMove(snakeLocal, Point(0, -1));
    }
    else if (tryDirection == 2){
        afterstatus = game.SnakeMove(snakeLocal, Point(-1, 0));
    }
    else if (tryDirection == 3){
        afterstatus = game.SnakeMove(snakeLocal, Point(0, 1));
    }
    else if (tryDirection == 4){
        afterstatus = game.SnakeMove(snakeLocal, Point(1, 0));
    }
    if (afterstatus == -4) {
        snakeLocal.SetTryDirection(snakeLocal.GetDirection());
        return onTimeOut(snakeLocal);
    }
    snakeLocal.SetDirection(tryDirection);
    qDebug() << "afterstatus = " << afterstatus << Qt::endl;
    return afterstatus;
}

long long getCurrentTime()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void GameWindow::TimelyAccess() {

    QString m_ObjectName = sender()->objectName();
    qDebug() << "sender is :" << m_ObjectName << Qt::endl;
    qDebug() << "need sender is :" << timer[0] << Qt::endl;
    int index = 0;
    while (1) {
        qDebug() << "while sender is :" << timer[index]->objectName() << Qt::endl;
        if (m_ObjectName == timer[index++]->objectName()) break;

    }
    ListNode<Snake> *head = game.snakeList.Find(index - 2);
    Snake &snakeLocal = head->data;
    timer[index]->start(snakeLocal.GetRefreshTime()); /*更新刷新时间*/
    if (onTimeOut(snakeLocal) < 0 && snakeLocal.GetLife() > 0){
        snakeLocal.SetLife(snakeLocal.GetLife() - 1); /*这条蛇的生命值-1*/
        if (snakeLocal.GetLife() <= 0) { /*第一次探测到死亡时，存活蛇-1，并且蛇本体全部删除*/
            game.lived_snakes --;
            while (snakeLocal.GetBody().GetCurrentLength() > 0) {
                snakeLocal.GetBody().Delete();
            }
            timer[index]->stop();
        }
    }
    if (game.lived_snakes == 0) { /*如果游戏中的蛇全部死亡，进入结算界面*/
        ChangeToGameOverWindow();
        stopped = true;
        return;
    }
    update();
}
void GameWindow::TimelyAccess0() {
    //long long int now_time = getCurrentTime();
    ListNode<Snake> *head = game.snakeList.head->next;
    Snake &snakeLocal = head->data;
    timer[0]->start(snakeLocal.GetRefreshTime()); /*更新刷新时间*/
    if (onTimeOut(snakeLocal) < 0 && snakeLocal.GetLife() > 0){
        snakeLocal.SetLife(snakeLocal.GetLife() - 1); /*这条蛇的生命值-1*/
        if (snakeLocal.GetLife() <= 0) { /*第一次探测到死亡时，存活蛇-1，并且蛇本体全部删除*/
            game.lived_snakes --;
            while (snakeLocal.GetBody().GetCurrentLength() > 0) {
                snakeLocal.GetBody().Delete();
            }
            timer[0]->stop();
        }
    }
    if (game.lived_snakes == 0) { /*如果游戏中的蛇全部死亡，进入结算界面*/
        ChangeToGameOverWindow();
        stopped = true;
        return;
    }

    update();
}

void GameWindow::TimelyAccess1() {
    if (game.snakeList.head->next->next == NULL) return;
    ListNode<Snake> *head = game.snakeList.head->next->next;
    Snake &snakeLocal = head->data;
    timer[1]->start(snakeLocal.GetRefreshTime()); /*更新刷新时间*/
    if (onTimeOut(snakeLocal) < 0 && snakeLocal.GetLife() > 0){
        snakeLocal.SetLife(snakeLocal.GetLife() - 1); /*这条蛇的生命值-1*/
        if (snakeLocal.GetLife() <= 0) { /*第一次探测到死亡时，存活蛇-1，并且蛇本体全部删除*/
            game.lived_snakes --;
            while (snakeLocal.GetBody().GetCurrentLength() > 0) {
                snakeLocal.GetBody().Delete();
            }
            timer[1]->stop();
        }
        else {
            int length = snakeLocal.GetBody().GetCurrentLength();
            while (snakeLocal.GetBody().GetCurrentLength() > 0) {
                snakeLocal.GetBody().Delete();
            }
            for (int i = 0; i < length; i++) {
                snakeLocal.GetBody().Insert(Point(0,i));
                snakeLocal.SetDirection(1);
                snakeLocal.SetTryDirection(1);
            }

        }
    }
    if (game.lived_snakes == 0) { /*如果游戏中的蛇全部死亡，进入结算界面*/
        ChangeToGameOverWindow();
        stopped = true;
        return;
    }
    update();
}

void GameWindow::TimelyAccess2() {
    if (game.snakeList.head->next->next == NULL) return;
    if (game.snakeList.head->next->next->next == NULL) return;
    ListNode<Snake> *head = game.snakeList.head->next->next->next;
    Snake &snakeLocal = head->data;
    timer[2]->start(snakeLocal.GetRefreshTime()); /*更新刷新时间*/
    if (onTimeOut(snakeLocal) < 0 && snakeLocal.GetLife() > 0){
        snakeLocal.SetLife(snakeLocal.GetLife() - 1); /*这条蛇的生命值-1*/
        if (snakeLocal.GetLife() <= 0) { /*第一次探测到死亡时，存活蛇-1，并且蛇本体全部删除*/
            game.lived_snakes --;
            while (snakeLocal.GetBody().GetCurrentLength() > 0) {
                snakeLocal.GetBody().Delete();
            }
            timer[2]->stop();
        }
    }
    if (game.lived_snakes == 0) { /*如果游戏中的蛇全部死亡，进入结算界面*/
        ChangeToGameOverWindow();
        stopped = true;
        return;
    }
    update();
}

void GameWindow::Save(){
    timer[0]->stop();
    timer[1]->stop();
    timer[2]->stop();
    ChangeToSaverWindow();
}

void GameWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this); //创建painter，绑定此窗口
    windowMap.Init(&game, Point(80,0), Point(560,480)); //绑定game,设置左上角和右下角
    windowMap.Draw(&painter); //绑定painter


}
