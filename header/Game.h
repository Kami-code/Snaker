#pragma once


#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include "LinkList.h"

#define maxSnake 100

/* 定义场地的主类，这部分需要独立于图形界面，提供游戏的接口。Move()*/
class Background{
private:
    int width = 30;             //场景横向网格格子数
    int height = 30;            //场景纵向网格格子数
    Point **ground = NULL;        //二维数组，存放网格状态
public:

    Background();

    void ClearGround();
    void CleanSnakeGround();
    void SetGround();
    void SetWidth(int);
    void SetHeight(int);
    int GetWidth();
    int GetHeight();
    bool IsConfined(Point);
    Point GetRandomVaildPoint();
    Point GetStatus(Point);
    Point CreateFood(int);         //创造出食物
    void CreateWall();         //创造出墙壁
    Point ** GetGround();
    Point FindFood(int, int);
    Background& operator = (const Background&);
};

/*
 * 定义蛇的主类，这部分需要独立于图形界面，提供最终的训练API接口和测试API接口，便于pytorch的调用。注意训练API接口为了效率考量，必须速度足够快，无须显示图形界面。
 * Move()
 */

class  Snake{
private:
    LinkList<Point> bodyList;       //存放每个关节的结点
    unsigned int id;
    int direction = 4;              //存放真实方向
    int trySetDirection = 4;        //存放尝试方向
    int refreshTime;           //存放刷新率
    static const int defaultRefreshTime = 50;
    static const int defaultLife = 1;
    int life;                   //存放剩余生命值
    int score = 0;                  //存放得分
    bool autoMove = false;
public:
    Snake& operator = (const Snake&);
    int errorStatus = 0;
    LinkList<Point> &GetBody();
    LinkList<Point> *GetBodyAddr();
    int GetDirection();
    int GetTryDirection();
    int GetRefreshTime();
    int GetLife();
    int GetScore();
    int GetID();
    bool GetAutoMove();
    void SetDirection(int);
    void SetTryDirection(int);
    void SetRefreshTime(int);
    void SetLife(int);
    void SetScore(int);
    void SetAutoMove(bool);
    Snake();
    Snake(Point);
    Point Position();
    void AppendBody(Point);
    void Move(Point);
    void Move(Point, int, int);
    void Clear(Background*,Point);
    pair<pair<float,float>,pair<float,float>> GetMeanAndVariance(Point);
};

class Setting{
private:
    bool showFigure = true;
    bool showAudio = false;
    bool penetrableness = true;
    int livedSnakeStatus = 1;
    int snakeNumber = 1;
    int aiSnakeMode = 0;
    int minRefreshTime = 10;
public:
    bool GetShowFigure();
    bool GetShowAudio();
    bool GetPenetrableness();
    int GetSnakeNumber();
    int GetLivedSnakeStatus();
    int GetAiSnakeMode();
    int GetMinRefreshTime();
    void SetShowFigure(bool);
    void SetShowAudio(bool);
    void SetPenetrableness(bool);
    void SetSnakeNumber(int);
    void SetLivedSnakeStatus(int);
    void SetAISnakeMode();

};

class Game{
public:
    Background background;      //存放地图类
    LinkList<Snake> snakeList;  //存放蛇的链表（多人游戏）
    Setting setting;
    int SnakeMove(Snake&, Point, bool MoveFlag = true);
    void ReInit();
};



#endif // SNAKE_H
