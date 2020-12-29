#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <iostream>
#include <queue>
#include <map>
#include "header/Game.h"
#include "header/LinkList.h"

void PrintPoint(Point localPoint, string s);
bool BFS (Point, Point, Background*, map<Point,Point>&); //BFS算法
float GetNorm(int snakeLength); //得到蛇对应长度的最大方差
int AIsnakeMove(Snake&, Background*);
#endif // ALGORITHM_H
