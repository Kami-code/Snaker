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

void WindowMap::Init(Game* game, Point leftUpPoint, Point rightDownPoint) {
    gameAddr = game;
    gameWidth = (*game).background.GetWidth();
    gameHeight = (*game).background.GetHeight();
    left = leftUpPoint.x;
    up = leftUpPoint.y;
    right = rightDownPoint.x;
    down = rightDownPoint.y;
    width = abs(left - right);
    height = abs(up - down);
    unitWidth = width / gameWidth;
    unitHeight = height / gameHeight;
}

void WindowMap::ChangePoint(Point leftUpPoint, Point rightDownPoint) {
    left = leftUpPoint.x;
    up = leftUpPoint.y;
    right = rightDownPoint.x;
    down = rightDownPoint.y;
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
            int status = (*gameAddr).background.GetGround()[i][j].x;
            if (status == 0) { //场地为空地的情况
                if (game.setting.GetShowFigure())
                    (*painter).setPen(QColor(255,255,255,255));
                else (*painter).setPen(QColor(0,0,0,255));
                (*painter).setBrush(Qt::NoBrush);
                (*painter).drawRect(unitWidth * i,unitHeight * j,unitWidth , unitHeight);/*绘制网格*/
            }
            else { //场地为食物的情况
                if (!game.setting.GetShowFigure()) {
                    (*painter).setPen(QColor(222,222,222,128));
                    (*painter).setBrush(QColor(0,255,255,128));
                    (*painter).drawRect(unitWidth * i,unitHeight * j,unitWidth , unitHeight);
                }
                else {
                    (*painter).setPen(Qt::NoPen);
                    (*painter).setBrush(QColor(0,255,255,128));
                    if (status != 1)(*painter).drawPixmap(unitWidth * i,unitHeight * j,unitWidth , unitHeight, resource.food[status - 2]);
                    else (*painter).drawPixmap(unitWidth * i,unitHeight * j,unitWidth , unitHeight, resource.wall);
                }
            }
        }
    }
    (*painter).setPen(Qt::NoPen);
    (*painter).setBrush(QColor(255,0,0,128));

    //绘制蛇
    ListNode<Snake> *localSnake = game.snakeList.head->next;

    int cnt = 0;
    int totalScore = 0;
    while (localSnake != NULL) {
        totalScore += localSnake->data.GetScore();
        ListNode<Point> *header = (localSnake->data).GetBody().head->next;
        if (localSnake->data.GetLife() < 0) continue;
        while (header != NULL) {
            if (!game.setting.GetShowFigure()) {
                (*painter).drawRect(unitWidth * header->data.x, unitHeight * header->data.y,unitWidth,unitHeight);
            }
            else {
                (*painter).drawPixmap(unitWidth * header->data.x, unitHeight * header->data.y,unitWidth , unitHeight, resource.character[cnt % resource.GetCharacterSize()]);
            }
            header = header->next;
        }
        localSnake = localSnake->next; cnt++;
    }
    QString text = QString::number(totalScore);
    QFont font("宋体",300,QFont::Bold, true);
    font.setPixelSize(50);
    (*painter).setFont(font);
    (*painter).setPen(Qt::blue);
    //获取这种字体情况下，文本的长度和高度
    QFontMetricsF fontMetrics(font);
    (*painter).translate(0, 0);
    //(*painter).scale(width / 200 , height / 200);

    (*painter).drawText(0, 0, QString::number(totalScore));
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
    (*gameAddr).background.GetGround()[colIndex][rowIndex].x ++;
}

/*
 * function: GameStopped
 * Description: Reverse the flag stopped. Stop or restart the game according to the flag.
 */
void GameWindow::GameStopped(){
    if (stopped) {
        for (unsigned i = 0; i < timerVector.size(); ++i) {
            timerVector[i]->start(300);
        }
        stopButton->setText("暂停");
        if(game.setting.GetShowAudio())QSound::play(":/audio/audio/eatnormal.wav");
     }
    else if (!stopped) {
        for (unsigned i = 0; i < timerVector.size(); ++i) {
            timerVector[i]->stop();
        }
        stopButton->setText("开始");
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

    returnButton = new QPushButton(this);
    returnButton->setText("切换到主窗口");
    returnButton->move(QPoint(50, 50));
    connect(returnButton, &QPushButton::released, this, &GameWindow::ChangeToDesktop);

    stopButton = new QPushButton(this);
    stopButton->setText("开始");
    stopButton->move(QPoint(100, 50));
    connect(stopButton, &QPushButton::released, this, &GameWindow::GameStopped);

    saveButton = new QPushButton(this);
    saveButton->setText("存档");
    saveButton->move(QPoint(150, 50));
    connect(saveButton, &QPushButton::released, this, &GameWindow::Save);

    for (int i = 0; i < 100; i++) {
        QTimer *localTimer;
        localTimer = new QTimer(this);
        localTimer->setObjectName(QString(i));
        qDebug() << localTimer->objectName() << Qt::endl;
        connect(localTimer, &QTimer::timeout, this, QOverload<>::of(&GameWindow::TimelyAccess)); //定周期调用move函数
        localTimer->stop();
        timerVector.push_back(localTimer);
    }
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

void GameWindow::resizeEvent(QResizeEvent *event) {
    int width = geometry().width();
    int height = geometry().height();
    windowMap.ChangePoint(Point(width / 5, height / 5), Point(4 * width / 5, 4 * height / 5));
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
    for (unsigned i = 0; i < timerVector.size(); ++i) {
        timerVector[i]->stop();
    }
    stopButton->setText("开始");
    emit SignalChangeToDesktop();
}

void GameWindow::ChangeToGameOverWindow()
{

    stopButton->setText("开始");
    if(game.setting.GetShowAudio())QSound::play(":/audio/audio/failure2.wav");
    emit SignalChangeToGameOverWindow();
//    qDebug()  << "emit gameover!";
}

void GameWindow::ChangeToSaverWindow()
{
    for (unsigned i = 0; i < timerVector.size(); ++i) {
        timerVector[i]->stop();
    }

    stopButton->setText("开始");
    emit SignalChangeToSaverWindow();
}

extern int AIsnakeMove(Snake &, Background*);

int onTimeOut(Snake& snakeLocal) //定时器事件触发绑定
{
    if (snakeLocal.GetLife() <= 0) return -5;

    ListNode <Snake> *nowSnake = game.snakeList.head->next;
    game.background.CleanSnakeGround();
    while (nowSnake != NULL) {
        ListNode <Point> *nowPoint = nowSnake->data.GetBody().head->next;
        while (nowPoint != NULL) {
            if (game.background.IsConfined(nowPoint->data) == false) {
                nowPoint = nowPoint->next;
                continue;
            }
            game.background.GetGround()[nowPoint->data.x][nowPoint->data.y].y = 1;
            nowPoint = nowPoint->next;
        }
        nowSnake = nowSnake->next;
    }


    if (snakeLocal.GetAutoMove() == true)snakeLocal.SetTryDirection(AIsnakeMove(snakeLocal, &game.background) + 1);
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
    if (snakeLocal.GetAutoMove() == false && afterstatus == -4) {
        snakeLocal.SetTryDirection(snakeLocal.GetDirection());
        return onTimeOut(snakeLocal);
    }
    snakeLocal.SetDirection(tryDirection);
    qDebug() << "afterstatus = " << afterstatus << Qt::endl;
    return afterstatus;
}

void GameWindow::TimelyAccess() {

    QString m_ObjectName = sender()->objectName();
    int index = 0;
    while (1) {
        if (m_ObjectName == timerVector[index]->objectName()) break;
        index = index + 1;
    }
    if (index >= game.snakeList.GetCurrentLength()) {
        timerVector[index]->stop();
        return;
    }
    ListNode<Snake> *head = game.snakeList.Find(index + 2);
    qDebug() << head->data.GetLife() << Qt::endl;
    Snake &snakeLocal = head->data;
    if(snakeLocal.GetLife() <= 0) {
        timerVector[index]->stop();
        return;
    }
    timerVector[index]->start(max(snakeLocal.GetRefreshTime(), game.setting.GetMinRefreshTime())); /*更新刷新时间*/
    if (onTimeOut(snakeLocal) < 0 && snakeLocal.GetLife() > 0){
        snakeLocal.SetLife(snakeLocal.GetLife() - 1); /*这条蛇的生命值-1*/
        if (snakeLocal.GetLife() <= 0) { /*第一次探测到死亡时，存活蛇-1，并且蛇本体全部删除*/
            game.setting.SetLivedSnakeStatus(game.setting.GetLivedSnakeStatus() - 1);
            while (snakeLocal.GetBody().GetCurrentLength() > 0) {
                snakeLocal.GetBody().Delete();
            }
            if (game.setting.GetShowAudio()) QSound::play(":/audio/audio/death.wav");

            timerVector[index]->stop();
        }
        else {
            int length = snakeLocal.GetBody().GetCurrentLength();
            while (snakeLocal.GetBody().GetCurrentLength() > 0) {
                snakeLocal.GetBody().Delete();
            }
            for (int i = 0; i < length; i++) {
                if (game.setting.GetPenetrableness() == true)snakeLocal.GetBody().Insert(Point(0,-i));
                else snakeLocal.GetBody().Insert(Point(1,1-i));
                snakeLocal.SetDirection(4);
                snakeLocal.SetTryDirection(4);
            }
            if (game.setting.GetShowAudio()) QSound::play(":/audio/audio/revive.wav");
        }
    }
    if (game.setting.GetLivedSnakeStatus() <= 0) { /*如果游戏中的蛇全部死亡，进入结算界面*/
        ChangeToGameOverWindow();
        stopped = true;
        return;
    }
    update();
}

void GameWindow::Save(){
    for (unsigned i = 0; i < timerVector.size(); ++i) {
        timerVector[i]->stop();
    }
    ChangeToSaverWindow();
}

void GameWindow::paintEvent(QPaintEvent *)
{
    ListNode<Snake> *localSnake = game.snakeList.head->next;
    if (stopped == false) {
        int count = 0;
        while (localSnake != NULL) {
            if (localSnake->data.GetLife() <= 0) timerVector[count]->stop();
            localSnake = localSnake->next;
            count++;
        }
    }
    QPainter painter(this); //创建painter，绑定此窗口
    int width = geometry().width();
    int height = geometry().height();
    windowMap.Init(&game, Point(width / 5, height / 5), Point(4 * width / 5, 4 * height / 5)); //绑定game,设置左上角和右下角
    windowMap.Draw(&painter); //绑定painter
}
