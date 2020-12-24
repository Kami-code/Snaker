#include "Saver.h"

Saver::Saver(){
    saved[0] = false;saved[1] = false;saved[2] = false;
}

Saver::Saver(Game* gameAddr){
    background[0] = (*gameAddr).background;
    //snakeList[0] = (*gameAddr).snakeList;
}

int Saver::GetSaverNumber(){
    return saverNumber;
}

bool Saver::GetSaved(int index){
    return saved[index];
}

void Saver::SetSaved(int index, bool setting) {
    saved[index] = setting;
}

void Saver::Save(int index, Game* gameAddr) {
    //
    //snake->data.getBody().Show();
    background[index] = (*gameAddr).background; /*得到背景*/

    while (snakeList[index].GetCurrentLength() != 0) snakeList[index].Delete();

    ListNode<Snake> *firstSnake = (*gameAddr).snakeList.head->next;
    while (firstSnake != NULL) { /*处理每条蛇的情况*/
        Snake localSnake;
        localSnake.SetLife(firstSnake->data.GetLife());
        localSnake.SetDirection(firstSnake->data.GetDirection());
        localSnake.SetRefreshTime(firstSnake->data.GetRefreshTime());
        localSnake.SetTryDirection(firstSnake->data.GetTryDirection());

        while ((localSnake).GetBody().GetCurrentLength() != 0) (localSnake).GetBody().Delete();
        for (int i = 2; i <= firstSnake->data.GetBody().GetCurrentLength() + 1; ++i) {
            if (firstSnake->data.GetBody().Find(i) == NULL) break;
            //qDebug() << "data = ("<< snakeLocal->data.getBody().Find(i)->data.x << " , " << snakeLocal->data.getBody().Find(i)->data.y << " ) " << Qt::endl;
            (localSnake).GetBody().Insert(firstSnake->data.GetBody().Find(i)->data); //一定要插入元素，这样才可以新开内存，插入指针会导致析构的时候出问题
        }
        qDebug() << "start saving!"<< Qt::endl;
        localSnake.GetBody().Show();
        qDebug() << "end saving!"<< Qt::endl;
        ListNode<Snake> *tmpNode = new ListNode<Snake>(localSnake, NULL);
        snakeList[index].Insert(tmpNode);
        firstSnake = firstSnake->next;
    }

    //snakes[index].getBody().Show();

}

void Saver::Load(int index, Game* gameAddr) {
    (*gameAddr).background = background[index];

    while ((*gameAddr).snakeList.GetCurrentLength() != 0) (*gameAddr).snakeList.Delete();

    ListNode<Snake> *firstSnake = snakeList[index].head->next;
    while (firstSnake != NULL) { /*处理每条蛇的情况*/
        Snake localSnake;
        localSnake.SetLife(firstSnake->data.GetLife());
        localSnake.SetDirection(firstSnake->data.GetDirection());
        localSnake.SetRefreshTime(firstSnake->data.GetRefreshTime());
        localSnake.SetTryDirection(firstSnake->data.GetTryDirection());
        while ((localSnake).GetBody().GetCurrentLength() != 0) (localSnake).GetBody().Delete();
        for (int i = 2; i <= firstSnake->data.GetBody().GetCurrentLength() + 1; ++i) {
            if (firstSnake->data.GetBody().Find(i) == NULL) break;
            //qDebug() << "data = ("<< snakeLocal->data.getBody().Find(i)->data.x << " , " << snakeLocal->data.getBody().Find(i)->data.y << " ) " << Qt::endl;
            (localSnake).GetBody().Insert(firstSnake->data.GetBody().Find(i)->data); //一定要插入元素，这样才可以新开内存，插入指针会导致析构的时候出问题
        }
        localSnake.GetBody().Show();
        ListNode<Snake> *tmpNode = new ListNode<Snake>(localSnake, NULL);
        (*gameAddr).snakeList.Insert(tmpNode);
        firstSnake = firstSnake->next;
    }
}
