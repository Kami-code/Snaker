/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "GameWindow.h"
#include "game.h"
#include "LinkList.h"
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QSound>

extern Game game;

WindowMap::WindowMap(){

}

WindowMap::WindowMap(Game* game, Point a, Point b) {
    init(game, a, b);
}

void WindowMap::init(Game* game, Point a, Point b) {
    gameAddr = game;
    gameWidth = (*game).background.getWidth();
    gameHeight = (*game).background.getHeight();
    left = a.x;
    up = a.y;
    right = b.x;
    down = b.y;
    width = abs(left - right);
    height = abs(up - down);
    unitWidth = width / gameWidth;
    unitHeight = height / gameHeight;
}

void WindowMap::draw(QPainter *painter) {
    (*painter).setRenderHint(QPainter::Antialiasing);
    (*painter).translate(left, up);
    //painter.scale(side / 200.0, side / 200.0);
    QPixmap food, image1, image2, image3;
    food.load(":/image/head2.png");
    image1.load(":/image/head4.png");
    image2.load(":/image/head1.png");
    image3.load(":/image/head3.png");
    for (int i = 0; i < gameWidth; i ++) {
        for (int j = 0; j < gameHeight; j ++) {
            int status = (*gameAddr).background.getGround()[i][j];
            if (status == 0) {
                (*painter).setPen(QColor(255,255,255,255));
                (*painter).setBrush(Qt::NoBrush);
                (*painter).drawRect(unitWidth * i,unitHeight * j,unitWidth , unitHeight);
            }
            else if (status == 1) {
                if (!game.showFigure) {
                    (*painter).setPen(QColor(222,222,222,128));
                    (*painter).setBrush(QColor(0,255,255,128));
                    (*painter).drawRect(unitWidth * i,unitHeight * j,unitWidth , unitHeight);
                }
                else {
                    (*painter).setPen(Qt::NoPen);
                    (*painter).setBrush(QColor(0,255,255,128));
                    (*painter).drawPixmap(unitWidth * i,unitHeight * j,unitWidth , unitHeight, food);
                }
            }
        }
    }
    (*painter).setPen(Qt::NoPen);
    (*painter).setBrush(QColor(255,0,0,128));

    ListNode<Snake> *head = game.snakeList.head->next;
    int cnt = 0;
    while (head != NULL) {
        ListNode<Point> *header = (head->data).getBody().head->next;
        while (header != NULL) {
            if (!game.showFigure) {
                (*painter).drawRect(unitWidth * header->data.x, unitHeight * header->data.y,unitWidth,unitHeight);
            }
            else {
                if (cnt == 1)(*painter).drawPixmap(unitWidth * header->data.x, unitHeight * header->data.y,unitWidth , unitHeight, image1);
                else if (cnt == 2) (*painter).drawPixmap(unitWidth * header->data.x, unitHeight * header->data.y,unitWidth , unitHeight, image2);
                else (*painter).drawPixmap(unitWidth * header->data.x, unitHeight * header->data.y,unitWidth , unitHeight, image3);
            }
            header = header->next;
        }
        head = head->next; cnt++;
    }

}

void WindowMap::leftClicked(Point a){
    if (a.x < left || a.x > right || a.y < up || a.y > down) return;
    int rowIndex = (a.y - up) / unitHeight;
    int colIndex = (a.x - left) / unitWidth;
    qDebug() << "change [" <<rowIndex << "][" << colIndex <<"]" << endl;
    (*gameAddr).background.getGround()[colIndex][rowIndex] ++;
}

void GameWindow::GameStopped(){
    if (stopped) {
        timer->start(300);
        at->setText("暂停");
     }
    else if (!stopped) {
        timer->stop();
        at->setText("开始");
    }
    stopped = !stopped;
}

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
{

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QPixmap(":/image/grass.png")));
    setPalette(pal);
    this->setParent(parent);
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

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&GameWindow::timelyAccess)); //定周期调用move函数
    stopped = true; //一开始蛇静止
    QTimer* timer2 = new QTimer(this); //不需要写为成员变量，自动消亡
    connect(timer2, &QTimer::timeout, this, QOverload<>::of(&GameWindow::update)); //定周期刷新界面
    timer2->start(500);

    setWindowTitle(tr("Game"));
}

void GameWindow::mousePressEvent(QMouseEvent *event)
{
    //左键按下
    if (event->button() == Qt::LeftButton)
    {
        Point nowPos = Point(event->pos());
        windowMap.leftClicked(nowPos);
        //将当前鼠标点击的数据插入到主要数据数组中
        update();        //调用绘图函数
    }
    //右键按下, 删除按下位置的元素
    else if (event->button() == Qt::RightButton)
    {

        update();       //调用绘图函数
    }
}

void GameWindow::keyPressEvent(QKeyEvent * event) {
    int key = event->key();
    int direction[3];
    memset(direction, 0, sizeof(direction));
    switch (key) {
    case Qt::Key_W:
        direction[1] = 1;
        break;
    case Qt::Key_A:
        direction[1] = 2;
        break;
    case Qt::Key_S:
        direction[1] = 3;
        break;
    case Qt::Key_D:
        direction[1] = 4;
        break;
    case Qt::Key_I:
        direction[2] = 1;
        break;
    case Qt::Key_J:
        direction[2] = 2;
        break;
    case Qt::Key_K:
        direction[2] = 3;
        break;
    case Qt::Key_L:
        direction[2] = 4;
        break;
    case Qt::Key_Up:
        direction[3] = 1;
        break;
    case Qt::Key_Left:
        direction[3] = 2;
        break;
    case Qt::Key_Down:
        direction[3] = 3;
        break;
    case Qt::Key_Right:
        direction[3] = 4;
        break;

    }

    ListNode<Snake> *head = game.snakeList.head->next;
    int cnt = 1;
    while (head != NULL) {
        if (direction[min(3, cnt)] != 0)(head->data).setTryDirection(direction[min(3, cnt)]);
        else (head->data).setTryDirection((head->data).getTryDirection());
        head = head->next;
        cnt++;
    }

}

GameWindow::~GameWindow(){

}

void GameWindow::ChangeToDesktop()
{
    timer->stop();
    emit SignalChangeToDesktop();
}

void GameWindow::ChangeToGameOverWindow()
{
    emit SignalChangeToGameOverWindow();
    if(game.showAudio)QSound::play(":/sound/failure.wav");
    qDebug()  << "emit gameover!";
}


int onTimeOut(Snake* snake) //定时器事件触发绑定
{
    int tryDirection = (*snake).getTryDirection();
    int afterstatus = 0;
    if (tryDirection == 1) {
        afterstatus = game.snakeMove(snake, Point(0, -1));
    }
    else if (tryDirection == 2){
        afterstatus = game.snakeMove(snake, Point(-1, 0));
    }
    else if (tryDirection == 3){
        afterstatus = game.snakeMove(snake, Point(0, 1));
    }
    else if (tryDirection == 4){
        afterstatus = game.snakeMove(snake, Point(1, 0));
    }
    if (afterstatus == -4) {
        (*snake).setTryDirection((*snake).getDirection());
        return onTimeOut(snake);
    }
    (*snake).setDirection(tryDirection);
    qDebug() << "afterstatus = " << afterstatus << endl;
    return afterstatus;
}

void GameWindow::timelyAccess() {
    ListNode<Snake> *head = game.snakeList.head->next;
    while (head != NULL) {
        if (onTimeOut(&(head->data)) < 0){
            ChangeToGameOverWindow();
            timer->stop();
            stopped = true;
            return;
        }
        head = head->next;
    }


    update();
}

void GameWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this); //创建painter，绑定此窗口
    windowMap.init(&game, Point(80,0), Point(560,480)); //绑定game
    windowMap.draw(&painter); //绑定painter


}
