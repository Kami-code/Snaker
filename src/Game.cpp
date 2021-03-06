#include <queue>
#include <QSound>
#include "header/Game.h"
#include "header/LinkList.h"

Background::Background() {
    ground = new Point*[width];
    for (int i = 0; i < width; ++i ) {
        ground[i] = new Point [height];
        for (int j = 0; j < height; ++j) ground[i][j] = Point(0,0);
    }
    ground[2][2].x = 1;
}

void Background::ClearGround() { //在width被删除前
    for (int i = 0; i < width; ++i ) {
        delete ground[i];
    }
    delete ground;
}

void Background::CleanSnakeGround() { //在width被删除前
    for (int i = 0; i < width; ++i ) {
        for (int j = 0; j < height; ++j) ground[i][j].y = 0;
    }
}

bool Background::IsConfined(Point localPoint) {
    if (localPoint.x < 0 || localPoint.x >= width || localPoint.y < 0 || localPoint.y >= height) return false;
    return true;
}

void Background::SetGround() {

    ground = new Point*[width];
    for (int i = 0; i < width; ++i ) {
        ground[i] = new Point [height];
        for (int j = 0; j < height; ++j) ground[i][j] = Point(0,0);
    }
}

Point Background::FindFood(int seed1 = 0, int seed2 = 0) {
    /*
    x = 2 普通食物
    x = 3 超级食物，生命+1
    x = 4 加速食物，速度+10%（刷新时间-10%）
    x = 5 减速食物，速度-10%（刷新时间+10%）
    */
    int mode = 0;

    if (mode == 0) {
        int randx = (hash<int>()(seed1 + seed2)) % width;
        int randy = (hash<int>()(seed1 + 2 * seed2)) % height;


        Point foodPoint(randx,randy);
        Point enumPoint(randx,randy);
        double minDist = 1e9;
        for (int i = 0; i < width; ++i ) {
            for (int j = 0; j < height; ++j) {
                if (ground[i][j].x >= 2 && ground[i][j].y == 0) {
                    double localDist = foodPoint.GetDist(Point(i,j));
                    if (localDist < minDist) {
                        minDist = localDist;
                        enumPoint = Point(i,j);
                    }
                }
            }
        }
        return enumPoint;
    }
    else {
        srand(time(NULL));
        int ans = 0, tmp = 0;
        for (int i = 0; i < width; ++i ) {
            for (int j = 0; j < height; ++j) {
                if (ground[i][j].x >= 2 && ground[i][j].y == 0) {
                    ans++;
                }
            }
        }
        if (ans == 0) return Point(-1,-1);
        int nowChosen = seed1 % ans + 1;
        for (int i = 0; i < width; ++i ) {
            for (int j = 0; j < height; ++j) {
                if (ground[i][j].x >= 2 && ground[i][j].y == 0) {
                    tmp++;
                    if (tmp == nowChosen) return Point(i,j);
                }
            }
        }
        return Point(-1,-1);
    }
}

Point Background::GetRandomVaildPoint() {
    int upper_bound = 500;
    for (int i = 0 ; i < upper_bound; ++i) {
        Point randomPoint(rand() % width, rand() % height);
        if (ground[randomPoint.x][randomPoint.y].x == 0 && ground[randomPoint.x][randomPoint.y].y == 0) {
            return randomPoint;
        }
    }
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (ground[i][j].x == 0 && ground[i][j].y == 0) {
                return Point(i,j);
            }
        }
    }
    return Point(-1,-1);
}

Point Background::GetStatus(Point localPoint){
    if (IsConfined(localPoint) == false) throw("range error");
    return ground[localPoint.x][localPoint.y];
}

void Background::CreateWall() {
    for (int i = 0; i < width; i++) {
        ground[i][0].x = 1;
        ground[i][height - 1].x = 1;
    }
    for (int j = 0; j < height; j++) {
        ground[0][j].x = 1;
        ground[width - 1][j].x = 1;
    }
    return;
}

Point Background::CreateFood(int mode = 1) { //O(N^2)
    if (mode == 1) {
        Point randPoint;
        bool isFind = false;
        srand(time(NULL));
        while (isFind == false) {
            randPoint = Point(rand() % width, rand() % height);
            if (GetStatus(randPoint).x  == 0 && GetStatus(randPoint).y == 0) isFind = true;
        }
        int score = rand() % 100;
        /*
        x = 2 普通食物
        x = 3 超级食物，生命+1
        x = 4 加速食物，速度+10%（刷新时间-10%）
        x = 5 减速食物，速度-10%（刷新时间+10%）
        */
        enum food {
            normal = 2,
            oneMoreLife = 3,
            speedUp = 4,
            speedDown = 5,
        };
        if (score < 60) {
            ground[randPoint.x][randPoint.y].x = normal;
        }
        else if (score < 90) {
            ground[randPoint.x][randPoint.y].x = speedUp;
        }
        else if (score < 98) {
            ground[randPoint.x][randPoint.y].x = speedDown;
        }
        else ground[randPoint.x][randPoint.y].x = oneMoreLife;
        return randPoint;
    }
    else {
    srand(time(NULL));
    Point randPoint(rand() % width, rand() % height);
    bool **visited;
    visited = new bool*[width];
    for (int i = 0; i < width; ++i ) {
        visited[i] = new bool [height];
        for (int j = 0; j < height; ++j) {
            visited[i][j] = false;
        }
    }
    //开始算法
    queue <Point> pointSet;
    pointSet.push(randPoint);
    visited[randPoint.x][randPoint.y] = true;
    Point tmp[4] = {Point(0,-1), Point(-1, 0), Point(0, 1), Point(1, 0)};
    //还没有处理满屏的情况
    while (pointSet.empty() != true) {
        Point nowPoint = pointSet.front();
        pointSet.pop();
        if (GetStatus(nowPoint).x  == 0 && GetStatus(nowPoint).y == 0) {
            randPoint = nowPoint;
            break;
        }
        int permutation[4] = {1,2,3,4};
        for(int i = 0; i < 4; ++i) {
            int tmp = permutation[i];
            int indexRandom = rand() % 4;
            permutation[i] = permutation[indexRandom];
            permutation[indexRandom] = tmp;
        }
        for (int i = 0; i < 4; ++i) {
            Point nextPoint = nowPoint + tmp[permutation[i]];
            if (IsConfined(nextPoint) == true && visited[nextPoint.x][nextPoint.y] == false) {
                pointSet.push(nextPoint);
                visited[nextPoint.x][nextPoint.y] = true;
            }
        }
    }
    //别忘了visited的析构
    for (int i = 0; i < width; ++i ) { delete visited[i];} delete visited;
    int score = rand() % 100;
    /*
    x = 2 普通食物
    x = 3 超级食物，生命+1
    x = 4 加速食物，速度+10%（刷新时间-10%）
    x = 5 减速食物，速度-10%（刷新时间+10%）
    */
    enum food {
        normal = 2,
        oneMoreLife = 3,
        speedUp = 4,
        speedDown = 5,
    };
    if (score < 60) {
        ground[randPoint.x][randPoint.y].x = normal;
    }
    else if (score < 90) {
        ground[randPoint.x][randPoint.y].x = speedUp;
    }
    else if (score < 98) {
        ground[randPoint.x][randPoint.y].x = speedDown;
    }
    else ground[randPoint.x][randPoint.y].x = oneMoreLife;
    return randPoint;
    }

}

int Background::GetWidth() {return width;}
int Background::GetHeight(){return height;}
Point **Background::GetGround(){return ground;}
void Background::SetHeight(int a){height = a;}
void Background::SetWidth(int a){width = a;}
Background& Background::operator = (const Background& a) {
    ClearGround();
    width = a.width;
    height = a.height;

    SetGround();
    for (int i = 0; i < width; ++i ) {
        for (int j = 0; j < height; ++j) ground[i][j] = a.ground[i][j];
    }
    return (*this);
}

/*snake的构造函数，创造头指针*/
Snake::Snake(){
    srand(time(NULL));
    id = rand();
    ListNode<Point> *head = new ListNode<Point>(Point(1,1), NULL);
    bodyList.Insert(head);
}

Snake::Snake(Point a){
    srand(time(NULL));
    id = rand();
    ListNode<Point> *head = new ListNode<Point>(a, NULL);
    bodyList.Insert(head);
}

Snake& Snake::operator = (const Snake& right) {
    bodyList = right.bodyList;
    direction = right.direction;
    trySetDirection = right.trySetDirection;
    refreshTime = right.refreshTime;
    life = right.life;
    return (*this);
}

Point Snake::Position() {
    return bodyList.head->next->data;
}

int Snake::GetDirection() {return direction;}
int Snake::GetTryDirection() {return trySetDirection;}
int Snake::GetRefreshTime() {return refreshTime;}
int Snake::GetLife() {return life;}
int Snake::GetScore() {return score;}
int Snake::GetID() {return id;}
bool Snake::GetAutoMove() {return autoMove;}
void Snake::SetDirection(int d) {direction = d;}
void Snake::SetTryDirection(int d) {trySetDirection = d;}
void Snake::SetRefreshTime(int d = defaultRefreshTime) {refreshTime = d;}
void Snake::SetLife(int d = defaultLife) {life = d;}
void Snake::SetScore(int d) {score = d;}
void Snake::SetAutoMove(bool d) {autoMove = d;}


void Snake::AppendBody(Point targetPos){
    ListNode<Point>* targetNode = new ListNode<Point>(targetPos, NULL);
    bodyList.Insert(targetNode, 1);
}

void Snake::Move(Point movePos) {
    Point nowPos = Position();
    Point targetPos =nowPos.move(movePos);
    ListNode<Point>* targetNode = new ListNode<Point>(targetPos, NULL);
    bodyList.Insert(targetNode, 1);
    bodyList.Delete();
}

void Snake::Move(Point movePos, int width, int height) {
    Point nowPos = Position();
    Point targetPos =nowPos.move(movePos, width, height);
    ListNode<Point>* targetNode = new ListNode<Point>(targetPos, NULL);
    bodyList.Insert(targetNode, 1);
    bodyList.Delete();
}

void Snake::Clear(Background *background = NULL,Point a = Point(1,1)){
    if (background != NULL) a = background->GetRandomVaildPoint();
    while (bodyList.GetCurrentLength() != 0) bodyList.Delete();
    ListNode<Point> *head = new ListNode<Point>(a, NULL);
    bodyList.Insert(head);
}



pair<pair<float,float>,pair<float,float>> Snake::GetMeanAndVariance(Point newHead) {
    pair<float,float> mean(newHead.x,newHead.y), variance(0,0);
    ListNode<Point> *localPoint = GetBody().head->next;
    while (localPoint != NULL) {
        if(localPoint->next == NULL) break; //最后一节不参加计算
        mean.first += localPoint->data.x;
        mean.second += localPoint->data.y;
        localPoint = localPoint->next;
    }
    mean.first /= GetBody().GetCurrentLength();
    mean.second /= GetBody().GetCurrentLength();
    localPoint = GetBody().head->next;
    while (localPoint != NULL) {
        if(localPoint->next == NULL) break; //最后一节不参加计算
        variance.first += (localPoint->data.x - mean.first) * (localPoint->data.x - mean.first);
        variance.second += (localPoint->data.y - mean.second) * (localPoint->data.y - mean.second);
        localPoint = localPoint->next;
    }
    variance.first += (newHead.x - mean.first) * (newHead.x - mean.first);
    variance.second += (newHead.x - mean.second) * (newHead.x - mean.second);
    variance.first /= GetBody().GetCurrentLength();
    variance.second /= GetBody().GetCurrentLength();
    variance.first = sqrt(variance.first);
    variance.second = sqrt(variance.second);
    return pair<pair<float,float>,pair<float,float>>(mean, variance);
}

LinkList<Point> &Snake::GetBody(){
    return bodyList;
}

LinkList<Point> *Snake::GetBodyAddr(){
    return &bodyList;
}



int Game::SnakeMove(Snake& snakeLocal, Point movePos, bool MoveFlag) {

    LinkList<Point> &snakeBody_ = snakeLocal.GetBody();
    snakeBody_.Show();
    Point nowPos_ = snakeLocal.Position();
    Point targetPos_ =nowPos_.move(movePos);
    /*check vaild*/
    if (targetPos_.x >= background.GetWidth() || targetPos_.y >= background.GetHeight() || targetPos_.x < 0 || targetPos_.y < 0) {
        if (setting.GetPenetrableness() == false) {
            if (MoveFlag) snakeLocal.errorStatus = -3;
            return -3; //如果要去的地方是边界，返回-3， 未完成要写场景穿透和游戏失败的情况
        }
        else {
            if (targetPos_.x < 0) targetPos_.x +=  background.GetWidth();
            if (targetPos_.y < 0) targetPos_.y +=  background.GetHeight();
            targetPos_.x %= background.GetWidth();
            targetPos_.y %= background.GetHeight();
        }
    }
    Point status = background.GetGround()[targetPos_.x][targetPos_.y];
    if ((status.x || status.y) == 0) { //场地为空地的情况，没有食物也没有蛇
        if (!MoveFlag) return 0;
        snakeLocal.Move(movePos, background.GetWidth(), background.GetHeight());
        //qDebug() << "snake[" << &snakeLocal << "] move to （ " << movePos.x << " , " << movePos.y << " ) " << Qt::endl;
        snakeLocal.errorStatus = 0;
        return 0;
    }
    //注意 食物的判定一定要在蛇的身体的后面，因为吃到食物“加头”的情况的前提是食物的地方的vaild的
    /*
    status = 2 普通食物
    status = 3 超级食物，生命+1
    status = 4 加速食物，速度+10%（刷新时间-10%）
    status = 5 减速食物，速度-10%（刷新时间+10%）
    */
    else if (status.x == 1 || status.x == 2 || status.x == 3 ||status.x == 4 || status.x == 5){
        enum groundStatus {
            wall = 1,
            normal = 2,
            oneMoreLife = 3,
            speedUp = 4,
            speedDown = 5,
        };

        switch (status.x)
        {
            case wall:
                if (MoveFlag)snakeLocal.errorStatus = -2;
                return -2; //如果要去的地方已经是墙了，返回-2，要继续写游戏失败的情况
            case normal:
                if(setting.GetShowAudio())QSound::play(":/audio/audio/eatnormal.wav");
                break;
            case oneMoreLife:
                snakeLocal.SetLife(snakeLocal.GetLife() + 1);
                if(setting.GetShowAudio())QSound::play(":/audio/audio/delicious.wav");
                break;
            case speedUp:
                snakeLocal.SetRefreshTime(int(0.8 * snakeLocal.GetRefreshTime()));
                if(setting.GetShowAudio())QSound::play(":/audio/audio/speedup.wav");
                break;
            case speedDown:
                snakeLocal.SetRefreshTime(int(1.2 * snakeLocal.GetRefreshTime()));
                if(setting.GetShowAudio())QSound::play(":/audio/audio/speeddown.wav");
                break;
        }

        if (!MoveFlag) return 1;
        snakeLocal.AppendBody(targetPos_); //加到蛇头
        background.CreateFood();
        background.GetGround()[targetPos_.x][targetPos_.y].x = 0; //先刷新再吃掉食物，以免刷在蛇头重叠处出现Bug
        snakeLocal.errorStatus = 1;
        snakeLocal.SetScore(snakeLocal.GetScore() + 1);
        return 1;
    }

    else if (status.y == 1){
        ListNode<Point>* head_ = snakeBody_.head->next;
        int cnt = 0;
        while (head_ != NULL) {
            if (head_->data.x == targetPos_.x && head_->data.y == targetPos_.y) {
                if (cnt == 1) {
                    return -4; //如果是回头的情况，防止用户的误触导致暴毙，返回-4无效化本次操作。
                }
                if (MoveFlag)snakeLocal.errorStatus = -2;
                return -2; //如果要去的地方已经是蛇的身体了，返回-2，要继续写游戏失败的情况
            }
            head_ = head_->next;
            cnt++;
        }
        return -1;
    }
    else {
        if (MoveFlag)snakeLocal.errorStatus = -1;//场地上不是纯地面 未完成，需要处理食物的情况
        return -1;
    }


}

void Game::ReInit(){
    background.ClearGround();
    background.SetGround();
    if (setting.GetPenetrableness() == false) {
        background.CreateWall();
    }
    for (int  i = 0 ; i< setting.GetSnakeNumber(); ++i) {
        background.CreateFood();
    }
    while (snakeList.GetCurrentLength() != 0) snakeList.Delete();
    while (snakeList.GetCurrentLength() != setting.GetSnakeNumber()) {
        Point headPoint = background.GetRandomVaildPoint();
        Snake* aa = new Snake(headPoint);
        background.GetGround()[headPoint.x][headPoint.y].y = 1;
        snakeList.Insert(*aa);
    }
    setting.SetLivedSnakeStatus(setting.GetSnakeNumber());
    ListNode<Snake> *localSnake = snakeList.head->next;
    int cnt = 0;
    while (localSnake != NULL) {
        cnt++;
        if (cnt > setting.GetSnakeNumber()) break;
        localSnake->data.Clear(&background);
        localSnake->data.SetRefreshTime();
        localSnake->data.SetLife();
        localSnake->data.SetTryDirection(4);
        localSnake->data.SetDirection(4);
        localSnake->data.errorStatus = 0;
        localSnake = localSnake->next;
    }


    if (setting.GetAiSnakeMode() == 1 && setting.GetSnakeNumber() > 1) {
        localSnake = snakeList.head->next->next;
        while (localSnake != NULL) {
            localSnake->data.SetAutoMove(true);
            localSnake = localSnake->next;
        }
    }
    else if (setting.GetAiSnakeMode() == 2) {
        localSnake = snakeList.head->next;
        while (localSnake != NULL) {
            localSnake->data.SetAutoMove(true);
            localSnake = localSnake->next;
        }
    }

}

bool Setting::GetShowFigure() {return showFigure;}
bool Setting::GetShowAudio() {return showAudio;}
bool Setting::GetPenetrableness() {return penetrableness;}
int Setting::GetSnakeNumber() {return snakeNumber;}
int Setting::GetMinRefreshTime() {return minRefreshTime;}
int Setting::GetLivedSnakeStatus() {return livedSnakeStatus;}
int Setting::GetAiSnakeMode() {return aiSnakeMode;}
void Setting::SetShowFigure(bool setting) {showFigure = setting;}
void Setting::SetShowAudio(bool setting) {showAudio = setting;}
void Setting::SetPenetrableness(bool setting) {penetrableness = setting;}
void Setting::SetSnakeNumber(int setting) {snakeNumber = setting;}
void Setting::SetLivedSnakeStatus(int setting) {livedSnakeStatus = setting;}
void Setting::SetAISnakeMode() {aiSnakeMode = (aiSnakeMode + 1) % 3;}
