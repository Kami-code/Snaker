#pragma once

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QSound>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtCore/QDebug>
#include "Resource.h"
#include "GameWindow.h"
#include "GameOverWindow.h"
#include "SettingWindow.h"
#include "SaverWindow.h"


class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

signals:

public slots:
    void myButtonReleasedSlot();
    void changeToGameWindow();
    void changeToGameOverWindow();
    void changeWindowToSettingWindow();
    void changeWindowToSaverWindow();
    void changeWindowToDesktop();
    void changeToGameWindowWithNoInit();
private:
    //Ui::MainWidgetClass ui;

    QPushButton * bt1;
    QPushButton * bt2;
    QPushButton * bt3;
    QPushButton * bt4;
    QPushButton * bt5;

    QSound desktopSound;
    QSound gameWindowSound;
    GameWindow * gameWindow;
    GameOverWindow * gameOverWindow;
    SettingWindow * settingWindow;
    SaverWindow * saverWindow;
};

#endif // MAINWIDGET_H
