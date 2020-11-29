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
    QPushButton * bt;
};


#endif // SUBWIDGET_H
