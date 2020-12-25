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
#pragma once
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QtWidgets/QPushButton>
#include "Game.h"
#include "Resource.h"
#include "LinkList.h"

class WindowMap
{
private:
    float size;
    float left;         /*左上角的x值*/
    float up;           /*左上角的y值*/
    float right;        /*右下角的x值*/
    float down;         /*右下角的y值*/
    float width;
    float height;
    float unitWidth;
    float unitHeight;
    float gameWidth;    /*场地横向格子数目*/
    float gameHeight;   /*场地纵向格子数目*/
    Game *gameAddr;     /*绑定Game*/
public:
    WindowMap();
    WindowMap(Game *, Point, Point);
    void Init(Game *, Point, Point);
    void Draw( QPainter *painter);
    void LeftClicked(Point);            /*屏幕被点击的情况，通过反映射找出*/
    Resource resource;                  /*绑定界面资源文件*/
};

class GameWindow : public QWidget
{
    Q_OBJECT

public:

    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();
    void ChangeToDesktop();
    void ChangeToGameOverWindow();
    void ChangeToSaverWindow();
    void GameStopped();
    void TimelyAccess();
    void TimelyAccess0();
    void TimelyAccess1();
    void TimelyAccess2();
    void Save();
signals:
    /*
     * 信号必须要signals关键字来声明
     * 信号没有返回值, 但可以有参数
     * 信号就是函数的声明, 无需定义
     * 使用: emit mySignal();
     * 信号可以被重载
     */
    void SignalChangeToDesktop();
    void SignalChangeToGameOverWindow();
    void SignalChangeToSaverWindow();
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
public slots:
    //void onTimeOut();

private:

    QPushButton * at,*bt, *ct;
    WindowMap windowMap;
    QTimer *timer[102];
    int timeCount = 0;
    bool stopped = true;
};



#endif
