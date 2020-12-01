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

signals:
    /**
     * 信号必须要signals关键字来声明
     * 信号没有返回值, 但可以有参数
     * 信号就是函数的声明, 无需定义
     * 使用: emit mySignal();
     * 信号可以被重载
     */
    void SignalChangeToDesktop();

public slots:

private:
    QPushButton * audioButton;
    QPushButton * figureButton;
    QPushButton * returnButton;
    QSlider * slider;
    QSlider * slider2;
};


#endif // SETTINGWINDOW_H
