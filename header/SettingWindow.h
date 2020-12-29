#pragma once

#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QSlider>

class SettingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = 0);
    ~SettingWindow();

    void ChangeToDesktop();
    void ChangeAudioSetting();
    void ChangeFigureSetting();
    void SetSnakes(int);
    void SetBackgroundSize(int);
    void ChangeAISnakeSetting();
    void ChangePenetrateSetting();
signals:
    void SignalChangeToDesktop();

public slots:

private:
    QPushButton * audioButton;
    QPushButton * figureButton;
    QPushButton * returnButton;
    QPushButton * aiSnakeButton;
    QPushButton * penetrateButton;

    QSlider * slider;
    QSlider * slider2;
};


#endif // SETTINGWINDOW_H
