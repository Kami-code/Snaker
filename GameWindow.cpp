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
#include "Game.h"
#include "LinkList.h"
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QSound>
#include <Resource.h>
#include "Saver.h"

extern Game game;
extern Saver saver;

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
    for (int i = 0; i < gameWidth; i ++) {
        for (int j = 0; j < gameHeight; j ++) {
            int status = (*gameAddr).background.getGround()[i][j] % 100;
            if (status == 0) {
                (*painter).setPen(QColor(255,255,255,255));
                (*painter).setBrush(Qt::NoBrush);
                (*painter).drawRect(unitWidth * i,unitHeight * j,unitWidth , unitHeight);
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
        ListNode<Point> *header = (head->data).getBody().head->next;
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

void WindowMap::leftClicked(Point a){
    if (a.x < left || a.x > right || a.y < up || a.y > down) return;
    int rowIndex = (a.y - up) / unitHeight;
    int colIndex = (a.x - left) / unitWidth;
    qDebug() << "change [" <<rowIndex << "][" << colIndex <<"]" << Qt::endl;
    (*gameAddr).background.getGround()[colIndex][rowIndex] ++;
}

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

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
{

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QPixmap(":/image/image/grass.png")));
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

    ct = new QPushButton(this);
    ct->setText("存档");
    ct->move(QPoint(150, 50));
    connect(ct, &QPushButton::released, this, &GameWindow::save);

    timer[0] = new QTimer(this);
    timer[1] = new QTimer(this);
    timer[2] = new QTimer(this);
    connect(timer[0], &QTimer::timeout, this, QOverload<>::of(&GameWindow::timelyAccess0)); //定周期调用move函数
    connect(timer[1], &QTimer::timeout, this, QOverload<>::of(&GameWindow::timelyAccess1)); //定周期调用move函数
    connect(timer[2], &QTimer::timeout, this, QOverload<>::of(&GameWindow::timelyAccess2)); //定周期调用move函数
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
    int tryDirection = snakeLocal.getTryDirection();
    int afterstatus = 0;
    if (tryDirection == 1) {
        afterstatus = game.snakeMove(snakeLocal, Point(0, -1));
    }
    else if (tryDirection == 2){
        afterstatus = game.snakeMove(snakeLocal, Point(-1, 0));
    }
    else if (tryDirection == 3){
        afterstatus = game.snakeMove(snakeLocal, Point(0, 1));
    }
    else if (tryDirection == 4){
        afterstatus = game.snakeMove(snakeLocal, Point(1, 0));
    }
    if (afterstatus == -4) {
        snakeLocal.setTryDirection(snakeLocal.getDirection());
        return onTimeOut(snakeLocal);
    }
    snakeLocal.setDirection(tryDirection);
    qDebug() << "afterstatus = " << afterstatus << Qt::endl;
    return afterstatus;
}

void GameWindow::timelyAccess0() {
    ListNode<Snake> *head = game.snakeList.head->next;

    while (head != NULL) {
        Snake &snakeLocal = head->data;
        timer[0]->start(snakeLocal.getRefreshTime());
        if (onTimeOut(snakeLocal) < 0){
            snakeLocal.setLife(snakeLocal.getLife() - 1);
            if (snakeLocal.getLife() <= 0) {
                ChangeToGameOverWindow();
                timer[0]->stop();
                stopped = true;
                return;
            }
        }
        head = head->next;
    }
    timer[0]->start();

    update();
}

void GameWindow::timelyAccess1() {
    if (game.snakeList.head->next->next == NULL) return;
    ListNode<Snake> *head = game.snakeList.head->next->next;

    while (head != NULL) {
        Snake &snakeLocal = head->data;
        timer[1]->start(snakeLocal.getRefreshTime());
        if (onTimeOut(snakeLocal) < 0){
            snakeLocal.setLife(snakeLocal.getLife() - 1);
            if (snakeLocal.getLife() <= 0) {
                ChangeToGameOverWindow();
                timer[1]->stop();
                stopped = true;
                return;
            }
        }
        head = head->next;
    }
    timer[1]->start();

    update();
}

void GameWindow::timelyAccess2() {
    if (game.snakeList.head->next->next == NULL) return;
    if (game.snakeList.head->next->next->next == NULL) return;
    ListNode<Snake> *head = game.snakeList.head->next->next->next;

    while (head != NULL) {
        Snake &snakeLocal = head->data;
        timer[2]->start(snakeLocal.getRefreshTime());
        if (onTimeOut(snakeLocal) < 0){
            snakeLocal.setLife(snakeLocal.getLife() - 1);
            if (snakeLocal.getLife() <= 0) {
                ChangeToGameOverWindow();
                timer[2]->stop();
                stopped = true;
                return;
            }
        }
        head = head->next;
    }
    timer[2]->start();

    update();
}

void GameWindow::save(){
    timer[0]->stop();
    timer[1]->stop();
    timer[2]->stop();
    ChangeToSaverWindow();

}

void GameWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this); //创建painter，绑定此窗口
    windowMap.init(&game, Point(80,0), Point(560,480)); //绑定game
    windowMap.draw(&painter); //绑定painter


}
