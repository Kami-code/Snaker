#pragma once


#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include "LinkList.h"
#include <vector>


/* 定义场地的主类，这部分需要独立于图形界面，提供游戏的接口。Move()*/
class Background{
private:
    int width = 30;
    int height = 30;
    int **ground = NULL;
public:
    Background();
    void clearGround();
    void setGround();
    void setWidth(int);
    void setHeight(int);
    int getWidth();
    int getHeight();
    int ** getGround();
    Background& operator = (const Background&);
};

/*
 * 定义蛇的主类，这部分需要独立于图形界面，提供最终的训练API接口和测试API接口，便于pytorch的调用。注意训练API接口为了效率考量，必须速度足够快，无须显示图形界面。
 * Move()
 */

class Snake{
private:
    LinkList<Point> bodyList;
    int direction = 4;
    int trySetDirection = 4;
    int refreshTime = 50;
    int life = 1e9;
public:
    Snake& operator = (const Snake&);
    int errorStatus = 0;
    LinkList<Point> &getBody();
    LinkList<Point> *getBodyAddr();
    int getDirection();
    int getTryDirection();
    int getRefreshTime();
    int getLife();
    void setDirection(int);
    void setTryDirection(int);
    void setRefreshTime(int);
    void setLife(int);
    Snake();
    Snake(Point);
    Point position();
    void appendBody(Point);
    void move(Point);
    void move(Point, int, int);
    void clear(Point);
};

class Game{
public:
    Background background;
    LinkList<Snake> snakeList;
    bool showFigure = true;
    bool showAudio = true;
    bool penetrate = true;

    int snakeMove(Snake&, Point);
    void reInit();
};



#endif // SNAKE_H
