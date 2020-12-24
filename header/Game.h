#pragma once


#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include "LinkList.h"


/* 定义场地的主类，这部分需要独立于图形界面，提供游戏的接口。Move()*/
class Background{
private:
    int width = 30;             //场景横向网格格子数
    int height = 30;            //场景纵向网格格子数
    int **ground = NULL;        //二维数组，存放网格状态
public:
    Background();
    void ClearGround();
    void SetGround();
    void SetWidth(int);
    void SetHeight(int);
    int GetWidth();
    int GetHeight();
    int ** GetGround();
    Background& operator = (const Background&);
};

/*
 * 定义蛇的主类，这部分需要独立于图形界面，提供最终的训练API接口和测试API接口，便于pytorch的调用。注意训练API接口为了效率考量，必须速度足够快，无须显示图形界面。
 * Move()
 */

class Snake{
private:
    LinkList<Point> bodyList;       //存放每个关节的结点
    int direction = 4;              //存放真实方向
    int trySetDirection = 4;        //存放尝试方向
    int refreshTime = 50;           //存放刷新率
    int life = 3;                   //存放剩余生命值
public:
    Snake& operator = (const Snake&);
    int errorStatus = 0;
    LinkList<Point> &GetBody();
    LinkList<Point> *GetBodyAddr();
    int GetDirection();
    int GetTryDirection();
    int GetRefreshTime();
    int GetLife();
    void SetDirection(int);
    void SetTryDirection(int);
    void SetRefreshTime(int);
    void SetLife(int);
    Snake();
    Snake(Point);
    Point Position();
    void AppendBody(Point);
    void Move(Point);
    void Move(Point, int, int);
    void Clear(Point);
};

class Game{
public:
    Background background;      //存放地图类
    LinkList<Snake> snakeList;  //存放蛇的链表（多人游戏）
    bool showFigure = true;
    bool showAudio = true;
    bool penetrate = true;
    int lived_snakes = 1;
    int SnakeMove(Snake&, Point);
    void ReInit();
};



#endif // SNAKE_H
