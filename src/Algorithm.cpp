#include <iostream>
#include <queue>
#include <map>
#include "header/Game.h"
#include "header/LinkList.h"

bool IsConfined(Point localPoint, int width, int height) {
    if (localPoint.x < 0 || localPoint.x >= width || localPoint.y < 0 || localPoint.y >= height) return false;
    return true;
}

void PrintPoint(Point localPoint, string s = "") {
    qDebug() << QString::fromStdString(s) <<":(" << localPoint.x << "," << localPoint.y << ")" << Qt::endl;
}

bool BFS (Point startPoint, Point endPoint, Background* background, map <Point, Point> &fatherMap) {
    if (background->IsConfined(endPoint) == false) return false;
    queue <Point> pointSet;
    bool **visited;
    int width = background->GetWidth();
    int height = background->GetHeight();
    visited = new bool*[width];
    for (int i = 0; i < width; ++i ) {
        visited[i] = new bool [height];
        for (int j = 0; j < height; ++j) {
            if (background->GetGround()[i][j].y == 0) visited[i][j] = false;
            else visited[i][j] = true;
        }
    }
    visited[endPoint.x][endPoint.y] = false;
    //开始算法
    bool isFind = false;    pointSet.push(startPoint);
    visited[startPoint.x][startPoint.y] = true;
    Point tmp[4] = {Point(0,-1), Point(-1, 0), Point(0, 1), Point(1, 0)};
    //PrintPoint(startPoint,"startPoint");
    //PrintPoint(endPoint,"endPoint");
    while (pointSet.empty() != true) {
        Point nowPoint = pointSet.front();
        pointSet.pop();
        if (nowPoint == endPoint) {isFind = true; break;} //找到了对应的路

        for (int i = 0; i < 4; ++i) {
            Point nextPoint = nowPoint + tmp[i];

            if (IsConfined(nextPoint, width, height) == true && visited[nextPoint.x][nextPoint.y] == false) {
                fatherMap[nextPoint] = nowPoint;

                pointSet.push(nextPoint);
                visited[nextPoint.x][nextPoint.y] = true;
            }
        }
    }
    //别忘了visited的析构
    for (int i = 0; i < width; ++i ) {
        delete visited[i];
    }
    delete visited;
    return isFind;
}

float GetNorm(int snakeLength){
    float ans = 0;
    if (snakeLength % 2 == 0){
        for (int i = 1; i <= snakeLength / 2; ++i) {
            ans += (i - 0.5) * (i - 0.5);
        }
    }
    else {
        for (int i = 1; i <= snakeLength / 2; ++i) {
            ans += (i) * (i);
        }
    }

    return sqrt(2) * sqrt(ans / snakeLength);
}

extern Game game;

int AIsnakeMove(Snake & localSnake, Background* background) {
    map <Point, Point> fatherMap;
    Point startPoint = localSnake.GetBody().head->next->data; //蛇头位置
    Point endPoint = background->FindFood();

    BFS(startPoint, endPoint, background, fatherMap);
    if (fatherMap.count(endPoint) != 0) {
        while(!(fatherMap[endPoint] == startPoint)) {
            endPoint = fatherMap[endPoint];
        }
    }

    Point tmp[4] = {Point(0,-1), Point(-1, 0), Point(0, 1), Point(1, 0)};
    srand(time(NULL));

    for (int i = 0; i < 4; ++i) {
        if ((endPoint - startPoint) == tmp[i]) {
            if (game.SnakeMove(localSnake, tmp[i], false) >= 0) return i;
        }
    }
    int index = rand() % 4;
    float cost = 0;
    qDebug() << "________" << Qt::endl;
    for (int i = 0; i < 4; ++i) {
        if (game.SnakeMove(localSnake, tmp[i], false) >= 0) {
            pair<pair<float,float>,pair<float,float>> meanAndVariance = localSnake.GetMeanAndVariance(startPoint+tmp[i]);
            pair<float,float> mean = meanAndVariance.first, variance = meanAndVariance.second;
            float score = sqrt(variance.first * variance.first + variance.second * variance.second) / GetNorm(localSnake.GetBody().GetCurrentLength());
            qDebug() << "score = " << score << Qt::endl;
            if (score > cost) {
                cost = score;
                index = i;
            }
        }
    }
    qDebug() << "________" << Qt::endl;
    return index;
}










