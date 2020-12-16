#include "Saver.h"

Saver::Saver(){
    saved[0] = false;saved[1] = false;saved[2] = false;
}

Saver::Saver(Game* gameAddr){
    background[0] = (*gameAddr).background;
    //snakeList[0] = (*gameAddr).snakeList;
}

int Saver::getSaverNumber(){
    return saverNumber;
}

bool Saver::getSaved(int index){
    return saved[index];
}

void Saver::setSaved(int index, bool setting) {
    saved[index] = setting;
}

void Saver::save(int index, Game* gameAddr) {
    //
    //snake->data.getBody().Show();
    background[index] = (*gameAddr).background; /*得到背景*/

    while (snakeList[index].getCurrentLength() != 0) snakeList[index].Delete();

    ListNode<Snake> *firstSnake = (*gameAddr).snakeList.head->next;
    while (firstSnake != NULL) { /*处理每条蛇的情况*/
        Snake localSnake;
        localSnake.setLife(firstSnake->data.getLife());
        localSnake.setDirection(firstSnake->data.getDirection());
        localSnake.setRefreshTime(firstSnake->data.getRefreshTime());
        localSnake.setTryDirection(firstSnake->data.getTryDirection());

        while ((localSnake).getBody().getCurrentLength() != 0) (localSnake).getBody().Delete();
        for (int i = 2; i <= firstSnake->data.getBody().getCurrentLength() + 1; ++i) {
            if (firstSnake->data.getBody().Find(i) == NULL) break;
            //qDebug() << "data = ("<< snakeLocal->data.getBody().Find(i)->data.x << " , " << snakeLocal->data.getBody().Find(i)->data.y << " ) " << Qt::endl;
            (localSnake).getBody().Insert(firstSnake->data.getBody().Find(i)->data); //一定要插入元素，这样才可以新开内存，插入指针会导致析构的时候出问题
        }
        qDebug() << "start saving!"<< Qt::endl;
        localSnake.getBody().Show();
        qDebug() << "end saving!"<< Qt::endl;
        ListNode<Snake> *tmpNode = new ListNode<Snake>(localSnake, NULL);
        snakeList[index].Insert(tmpNode);
        firstSnake = firstSnake->next;
    }

    //snakes[index].getBody().Show();

}

void Saver::load(int index, Game* gameAddr) {
    (*gameAddr).background = background[index];

    while ((*gameAddr).snakeList.getCurrentLength() != 0) (*gameAddr).snakeList.Delete();

    ListNode<Snake> *firstSnake = snakeList[index].head->next;
    while (firstSnake != NULL) { /*处理每条蛇的情况*/
        Snake localSnake;
        localSnake.setLife(firstSnake->data.getLife());
        localSnake.setDirection(firstSnake->data.getDirection());
        localSnake.setRefreshTime(firstSnake->data.getRefreshTime());
        localSnake.setTryDirection(firstSnake->data.getTryDirection());
        while ((localSnake).getBody().getCurrentLength() != 0) (localSnake).getBody().Delete();
        for (int i = 2; i <= firstSnake->data.getBody().getCurrentLength() + 1; ++i) {
            if (firstSnake->data.getBody().Find(i) == NULL) break;
            //qDebug() << "data = ("<< snakeLocal->data.getBody().Find(i)->data.x << " , " << snakeLocal->data.getBody().Find(i)->data.y << " ) " << Qt::endl;
            (localSnake).getBody().Insert(firstSnake->data.getBody().Find(i)->data); //一定要插入元素，这样才可以新开内存，插入指针会导致析构的时候出问题
        }
        localSnake.getBody().Show();
        ListNode<Snake> *tmpNode = new ListNode<Snake>(localSnake, NULL);
        (*gameAddr).snakeList.Insert(tmpNode);
        firstSnake = firstSnake->next;
    }
}
