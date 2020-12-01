#pragma once

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtCore/QDebug>
#include "GameOverWindow.h"
#include  "GameWindow.h"
#include "SettingWindow.h"
#include <QSound>
#include "Resource.h"


class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

signals:

public slots:
    void myButtonReleasedSlot();
    void changeWindowToSw1();
    void changeWindowToSw2();
    void changeWindowToSw3();
    void changeWindowToDesktop();
    void myDealSubWidget1Para(int, QString);

private:
    //Ui::MainWidgetClass ui;

    QPushButton * bt1;
    QPushButton * bt2;
    QPushButton * bt3;
    QPushButton * bt4;

    QSound desktopSound;
    QSound gameWindowSound;
    GameWindow * sw1;
    GameOverWindow *sw2;
    SettingWindow *sw3;

};

#endif // MAINWIDGET_H
