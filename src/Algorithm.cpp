#include "header/Algorithm.h"

void PrintPoint(Point localPoint, string s = "") {
    qDebug() << QString::fromStdString(s) <<":(" << localPoint.x << "," << localPoint.y << ")" << Qt::endl;
}

bool BFS (Point startPoint, Point endPoint, Background* background, map <Point, Point> &fatherMap) {
    if (background->IsConfined(endPoint) == false) return false;
    queue <Point> pointSet;
    bool **visited;
    int width = background->GetWidth();
    int height = background->GetHeight();
    //初始化visited数组
    visited = new bool*[width];
    for (int i = 0; i < width; ++i ) {
        visited[i] = new bool [height];
        for (int j = 0; j < height; ++j) {
            if (background->GetGround()[i][j].y == 0 && background->GetGround()[i][j].x != 1) visited[i][j] = false;
            //不存在蛇，也不存在墙的情况下，才允许visit
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
            Point nextPoint = nowPoint + tmp[i]; //产生上下左右的点的集合
            if (background->IsConfined(nextPoint) == true && visited[nextPoint.x][nextPoint.y] == false) {
                fatherMap[nextPoint] = nowPoint;
                pointSet.push(nextPoint);  //加入到队列中
                visited[nextPoint.x][nextPoint.y] = true;
            }
        }
    }
    while (pointSet.empty() != true) {pointSet.pop();}
    //别忘了visited的析构
    for (int i = 0; i < width; ++i ) {
        delete visited[i];
    }
    delete visited;
    return isFind; //返回是否找到路径
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

int AIsnakeMove(Snake & localSnake, Background* background) { //处理AI蛇的决策
    map <Point, Point> fatherMap;
    Point startPoint = localSnake.GetBody().head->next->data; //蛇头位置
    Point endPoint = background->FindFood(localSnake.GetID(), localSnake.GetBody().GetCurrentLength());

    BFS(startPoint, endPoint, background, fatherMap);
    if (fatherMap.count(endPoint) != 0) {
        while(!(fatherMap[endPoint] == startPoint)) {
            endPoint = fatherMap[endPoint];
        }
    }
    fatherMap.clear();

    Point tmp[4] = {Point(0,-1), Point(-1, 0), Point(0, 1), Point(1, 0)};
    srand(time(NULL));

    for (int i = 0; i < 4; ++i) { //如果在最短路径上有路，那么就返回最短路径
        if ((endPoint - startPoint) == tmp[i]) {
            if (game.SnakeMove(localSnake, tmp[i], false) >= 0) return i;
        }
    }
    int index = rand() % 4;
    float cost = 0;
    for (int i = 0; i < 4; ++i) { //尝试走可行的，使得蛇的方差最短的路径
        if (game.SnakeMove(localSnake, tmp[i], false) >= 0) {
            pair<pair<float,float>,pair<float,float>> meanAndVariance = localSnake.GetMeanAndVariance(startPoint+tmp[i]);
            pair<float,float> mean = meanAndVariance.first, variance = meanAndVariance.second;
            float score = sqrt(variance.first * variance.first + variance.second * variance.second) / GetNorm(localSnake.GetBody().GetCurrentLength());
            //qDebug() << "score = " << score << Qt::endl;
            if (score > cost) { //找到使得方差最大的决策
                cost = score;
                index = i;
            }
        }
    }
    return index; //返回决策0~4，分别对应上下左右
}
