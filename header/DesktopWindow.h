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


class DesktopWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DesktopWindow(QWidget *parent = 0);
    void InitButton();      //按键初始化
    void InitWindow();      //界面、信号和槽初始化
    ~DesktopWindow();

signals:

public slots:
    void ChangeToGameWindow();
    void ChangeToGameOverWindow();
    void ChangeToSettingWindow();
    void ChangeToSaverWindow();
    void ChangeToDesktopWindow();
    void ChangeToGameWindowWithNoInit();
private:
    //Ui::MainWidgetClass ui;

    QPushButton * closeButton;
    QPushButton * enterGameButton;
    QPushButton * settingButton;
    QPushButton * saverButton;

    QSound desktopSound;
    QSound gameWindowSound;
    GameWindow * gameWindow;
    GameOverWindow * gameOverWindow;
    SettingWindow * settingWindow;
    SaverWindow * saverWindow;
};

#endif // MAINWIDGET_H
