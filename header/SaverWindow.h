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
    void SignalChangeToDesktop();
    void SignalChangeToGameWindow();

public slots:

private:
    QPushButton * saverbutton[3];
    QPushButton * returnButton;
};


#endif // SAVERWINDOW_H
