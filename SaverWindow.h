#pragma once

#ifndef SAVERWINDOW_H
#define SAVERWINDOW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QSlider>

class SaverWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SaverWindow(QWidget *parent = 0);
    ~SaverWindow();

    void ChangeToDesktop();
    void LoadSaver0();
    void LoadSaver1();
    void LoadSaver2();
    void ChangeToGameWindow();

signals:
    /**
     * 信号必须要signals关键字来声明
     * 信号没有返回值, 但可以有参数
     * 信号就是函数的声明, 无需定义
     * 使用: emit mySignal();
     * 信号可以被重载
     */
    void SignalChangeToDesktop();
    void SignalChangeToGameWindow();

public slots:

private:
    QPushButton * saverbutton[3];
    QPushButton * returnButton;
};


#endif // SAVERWINDOW_H
