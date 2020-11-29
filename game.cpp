#include "game.h"
#include "LinkList.h"
#include <QSound>


Background::Background() {
    ground = new int*[width];
    for (int i = 0; i < width; ++i ) {
        ground[i] = new int [height];
        for (int j = 0; j < height; ++j) ground[i][j] = 0;
    }
    ground[2][2] = 1;
}

void Background::clearGround() { //在width被删除前
    for (int i = 0; i < width; ++i ) {
        delete ground[i];
    }
    delete ground;
}

void Background::setGround() {

    ground = new int*[width];
    for (int i = 0; i < width; ++i ) {
        ground[i] = new int [height];
        for (int j = 0; j < height; ++j) ground[i][j] = 0;
    }
    ground[2][2] = 1;
}

int Background::getWidth() {return width;}
int Background::getHeight(){return height;}
int **Background::getGround(){return ground;}
void Background::setHeight(int a){height = a;}
void Background::setWidth(int a){width = a;}
/*snake的构造函数，创造头指针*/
Snake::Snake(){
    ListNode<Point> *head = new ListNode<Point>(Point(1,1), NULL);
    bodyList.Insert(head);
}

Snake::Snake(Point a){
    ListNode<Point> *head = new ListNode<Point>(a, NULL);
    bodyList.Insert(head);
}

//void Snake

Point Snake::position() {
    return bodyList.head->next->data;
}

int Snake::getDirection() {return direction;}
int Snake::getTryDirection() {return trySetDirection;}
void Snake::setDirection(int d) {direction = d;}
void Snake::setTryDirection(int d) {trySetDirection = d;}


void Snake::appendBody(Point targetPos){
    ListNode<Point>* targetNode = new ListNode<Point>(targetPos, NULL);
    bodyList.Insert(targetNode, 1);
}

void Snake::move(Point movePos) {
    Point nowPos = position();
    Point targetPos =nowPos.move(movePos);

    ListNode<Point>* targetNode = new ListNode<Point>(targetPos, NULL);
    bodyList.Insert(targetNode, 1);
    bodyList.Delete();
    qDebug() << "_______Moving_______" <<endl;
    getBody().Show();
    qDebug() << "_______Moving_______" <<endl;
}

void Snake::clear() {
    while (bodyList.getCurrentLength() != 0) bodyList.Delete();
    ListNode<Point> *head = new ListNode<Point>(Point(1,1), NULL);
    bodyList.Insert(head);
}

LinkList<Point> &Snake::getBody(){
    return bodyList;
}

LinkList<Point> *Snake::getBodyAddr(){
    return &bodyList;
}


int Game::snakeMove(Snake* snakeAddr, Point movePos) {
    //----------------------multi------------------
    LinkList<Point> &snakeBody_ = (*snakeAddr).getBody();
    snakeBody_.Show();
    Point nowPos_ = (*snakeAddr).position();
    Point targetPos_ =nowPos_.move(movePos);
    /*check vaild*/
    if (targetPos_.x >= background.getWidth() || targetPos_.y >= background.getHeight() || targetPos_.x < 0 || targetPos_.y < 0) {
        (*snakeAddr).errorStatus = -3;
        return -3; //如果要去的地方是边界，返回-3， 未完成要写场景穿透和游戏失败的情况
    }
    int status_ = background.getGround()[targetPos_.x][targetPos_.y];
    if (status_ == 0) { //场地为空地的情况
        ListNode<Point>* head_ = snakeBody_.head->next;
        int cnt = 0;
        while (head_ != NULL) {
            if (head_->data.x == targetPos_.x && head_->data.y == targetPos_.y) {
                if (cnt == 1) {
                    return -4;
                }
                (*snakeAddr).errorStatus = -2;

                return -2; //如果要去的地方已经是蛇的身体了，返回-2，要继续写游戏失败的情况
            }
            head_ = head_->next;
            cnt++;
        }
    }
    //注意 食物的判定一定要在蛇的身体的后面，因为吃到食物“加头”的情况的前提是食物的地方的vaild的
    else if (status_ == 1){
        (*snakeAddr).appendBody(targetPos_);
        while (1) {
            int randx = rand() % (background.getWidth()), randy = rand() % (background.getHeight());
            if (background.getGround()[randx][randy] != 0) continue;
            ListNode<Point>* head_ = snakeBody_.head->next;
            int flag = 1;
            while (head_ != NULL) {
                if (head_->data.x == randx && head_->data.y == randy) flag = 0;
                head_ = head_->next;
            }
            if (flag == 0) continue;
            else if (flag == 1) {
                background.getGround()[randx][randy] = 1;
                break;
            }
        }
        background.getGround()[targetPos_.x][targetPos_.y] = 0; //吃掉食物
        if(showAudio)QSound::play(":/sound/eatnormal.wav");
        (*snakeAddr).errorStatus = 1;
        return 1;
    }
    else {
        (*snakeAddr).errorStatus = -1;//场地上不是纯地面 未完成，需要处理食物的情况
        return -1;
    }
    (*snakeAddr).move(movePos);
    qDebug() << "snake[" << snakeAddr << "] move to （ " << movePos.x << " , " << movePos.y << " ) " << endl;
    (*snakeAddr).errorStatus = 0;
    //----------------------multi------------------
    return 0;

}

void Game::reInit(){
    background.clearGround();
    background.setGround();
    ListNode<Snake> *head = snakeList.head->next;
    while (head != NULL) {
        Snake snake = head->data;
        snake.clear();
        snake.setTryDirection(4);
        snake.setDirection(4);
        snake.errorStatus = 0;
        head = head->next;
    }

}
