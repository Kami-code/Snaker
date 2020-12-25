#pragma once
#ifndef SUBWIDGET_H
#define SUBWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>

class GameOverWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameOverWindow(QWidget *parent = 0);
    ~GameOverWindow();

    void ChangeToDesktop();
    void ChangeToGameWindow();

signals:
    void SignalChangeToDesktop();
    void SignalChangeToGameWindow();

public slots:

private:
    QPushButton * bt;
};


#endif // SUBWIDGET_H
