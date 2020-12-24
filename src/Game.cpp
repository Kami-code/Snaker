#include <QSound>
#include "header/Game.h"
#include "header/LinkList.h"

Background::Background() {
    ground = new int*[width];
    for (int i = 0; i < width; ++i ) {
        ground[i] = new int [height];
        for (int j = 0; j < height; ++j) ground[i][j] = 0;
    }
    ground[2][2] = 1;
}

void Background::ClearGround() { //在width被删除前
    for (int i = 0; i < width; ++i ) {
        delete ground[i];
    }
    delete ground;
}

void Background::SetGround() {

    ground = new int*[width];
    for (int i = 0; i < width; ++i ) {
        ground[i] = new int [height];
        for (int j = 0; j < height; ++j) ground[i][j] = 0;
    }
    ground[2][2] = 1;
}

int Background::GetWidth() {return width;}
int Background::GetHeight(){return height;}
int **Background::GetGround(){return ground;}
void Background::SetHeight(int a){height = a;}
void Background::SetWidth(int a){width = a;}
Background& Background::operator = (const Background& a) {
    width = a.width;
    height = a.height;
    ClearGround();
    SetGround();
    for (int i = 0; i < width; ++i ) {
        for (int j = 0; j < height; ++j) ground[i][j] = a.ground[i][j];
    }
    return (*this);
}

/*snake的构造函数，创造头指针*/
Snake::Snake(){
    ListNode<Point> *head = new ListNode<Point>(Point(1,1), NULL);
    bodyList.Insert(head);
}

Snake::Snake(Point a){
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

//void Snake

Point Snake::Position() {
    return bodyList.head->next->data;
}

int Snake::GetDirection() {return direction;}
int Snake::GetTryDirection() {return trySetDirection;}
int Snake::GetRefreshTime() {return refreshTime;}
int Snake::GetLife() {return life;}
void Snake::SetDirection(int d) {direction = d;}
void Snake::SetTryDirection(int d) {trySetDirection = d;}
void Snake::SetRefreshTime(int d) {refreshTime = d;}
void Snake::SetLife(int d) {life = d;}

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

void Snake::Clear(Point a = Point(1,1)) {
    while (bodyList.GetCurrentLength() != 0) bodyList.Delete();
    bodyList.SetCurrentLength(0);
    ListNode<Point> *head = new ListNode<Point>(a, NULL);
    bodyList.Insert(head);
}

LinkList<Point> &Snake::GetBody(){
    return bodyList;
}

LinkList<Point> *Snake::GetBodyAddr(){
    return &bodyList;
}



int Game::SnakeMove(Snake& snakeLocal, Point movePos) {
    //----------------------multi------------------
    LinkList<Point> &snakeBody_ = snakeLocal.GetBody();
    snakeBody_.Show();
    Point nowPos_ = snakeLocal.Position();
    Point targetPos_ =nowPos_.move(movePos);
    /*check vaild*/
    if (targetPos_.x >= background.GetWidth() || targetPos_.y >= background.GetHeight() || targetPos_.x < 0 || targetPos_.y < 0) {
        if (!penetrate) {
            snakeLocal.errorStatus = -3;
            return -3; //如果要去的地方是边界，返回-3， 未完成要写场景穿透和游戏失败的情况
        }
        else {
            if (targetPos_.x < 0) targetPos_.x +=  background.GetWidth();
            if (targetPos_.y < 0) targetPos_.y +=  background.GetHeight();
            targetPos_.x %= background.GetWidth();
            targetPos_.y %= background.GetHeight();
        }
    }
    int status_ = background.GetGround()[targetPos_.x][targetPos_.y];
    if (status_%100 == 0) { //场地为空地的情况
        ListNode<Point>* head_ = snakeBody_.head->next;
        int cnt = 0;
        while (head_ != NULL) {
            if (head_->data.x == targetPos_.x && head_->data.y == targetPos_.y) {
                if (cnt == 1) {
                    return -4; //如果是回头的情况，防止用户的误触导致暴毙，返回-4无效化本次操作。
                }
                snakeLocal.errorStatus = -2;

                return -2; //如果要去的地方已经是蛇的身体了，返回-2，要继续写游戏失败的情况
            }
            head_ = head_->next;
            cnt++;
        }
    }
    //注意 食物的判定一定要在蛇的身体的后面，因为吃到食物“加头”的情况的前提是食物的地方的vaild的
    /*
    status = 1 普通食物，长度+1
    status = 2 超级食物，生命+1
    status = 3 加速食物， 速度+10%（刷新时间-10%）
    status = 4 减速食物， 速度-10%（刷新时间+10%）
    */
    else if (status_%100 == 1 || status_%100 == 2 || status_%100 == 3 ||status_%100 == 4){
        snakeLocal.AppendBody(targetPos_); //加到蛇头
        int **ground = background.GetGround();
        ListNode<Point>* head_ = snakeBody_.head->next;
        while (head_ != NULL) {
            ground[head_->data.x][head_->data.y] += 100;
            head_ = head_->next;
        }
        while (1) {
            int randx = rand() % (background.GetWidth()), randy = rand() % (background.GetHeight());
            if (background.GetGround()[randx][randy] != 0) continue;
            background.GetGround()[randx][randy] = rand() % 4 + 1; //刷新出四种食物
            break;
        }
        if (status_ % 100 == 2) {
            snakeLocal.SetLife(snakeLocal.GetLife() + 1);
        }
        if (status_ % 100 == 3) {
            snakeLocal.SetRefreshTime(int(0.5 * snakeLocal.GetRefreshTime()));
        }
        if (status_ % 100== 4) {
            snakeLocal.SetRefreshTime(int(1.2 * snakeLocal.GetRefreshTime()));
        }
        background.GetGround()[targetPos_.x][targetPos_.y] = 0; //先刷新再吃掉食物，以免刷在蛇头重叠处出现Bug
        if(showAudio)QSound::play(":/audio/audio/eatnormal.wav");
        snakeLocal.errorStatus = 1;
        return 1;
    }

    else {
        snakeLocal.errorStatus = -1;//场地上不是纯地面 未完成，需要处理食物的情况
        return -1;
    }
    snakeLocal.Move(movePos, background.GetWidth(), background.GetHeight());
    qDebug() << "snake[" << &snakeLocal << "] move to （ " << movePos.x << " , " << movePos.y << " ) " << Qt::endl;
    snakeLocal.errorStatus = 0;
    return 0;

}

void Game::ReInit(){
    background.ClearGround();
    background.SetGround();
    ListNode<Snake> *head = snakeList.head->next;
    while (head != NULL) {
        Snake &snake = head->data;
        snake.Clear();
        snake.SetTryDirection(4);
        snake.SetDirection(4);
        snake.errorStatus = 0;
        head = head->next;
    }


}
