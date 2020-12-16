#pragma once

#ifndef LINKLIST_H
#define LINKLIST_H

#include <QtGui>
#include <iostream>
#include <string>
using namespace std;

class Point {
public:
    int x, y;
    Point();
    Point(int x_, int y_):x(x_), y(y_){}
    Point (QPoint a):x(a.x()), y(a.y()){}
    Point move(Point movePos, int width, int height) {
        int tarx= x + movePos.x, tary = y + movePos.y;
        if (tarx < 0) tarx += width;
        if (tary < 0) tary += height;
        return Point(tarx % width, tary % height);
    }
    Point move(Point movePos) {
        return Point(x + movePos.x, y + movePos.y);
    }
    ostream& operator << (ostream &);
    bool operator > (const Point &) const;
    bool operator < (const Point &) const;
};



template<typename T>
class ListNode{
public:
    T data;
    ListNode* next = NULL;
    ListNode();
    ListNode(T s, ListNode* p = NULL):data(s), next(p){}
};

template<typename T>
class LinkList{
private:
    int currentLength = 0;
public:
    ListNode<T>* head = NULL;
    LinkList &operator = (const LinkList&);
    LinkList();
    LinkList(ListNode<T>* p):head(p){}
    ListNode<T>* Find(int) const;
    void Insert(T);
    void Insert(ListNode<T>*);
    void Insert(ListNode<T>*, int);
    void Delete();
    void Delete(int);
    void Show() const;
    void Show(int, int) const;
    int getCurrentLength() const;
    void setCurrentLength(int);
};

//模板类的实现要写在.h文件中

template<typename T>
LinkList<T>& LinkList<T>::operator = (const LinkList<T>& right) {
    while(currentLength != 0) (*this).Delete();
    for (int i = 1; i <= right.getCurrentLength(); ++i) {
        (*this).Insert((right.Find(i)->data)); //一定要插入元素，这样才可以新开内存，插入指针会导致析构的时候出问题
    }
    return (*this);
}

template<typename T>
LinkList<T>::LinkList() {
    T s = T();
    head = new ListNode<T>(s, NULL);
    currentLength = 0;
}

template<typename T>
int LinkList<T>::getCurrentLength() const{
    return currentLength;
}

template<typename T>
void LinkList<T>::setCurrentLength(int d){
    currentLength = d;
}

template<typename T>
ListNode<T>* LinkList<T>::Find(int index) const {
    ListNode<T>* now = head;
    for (int i = 0; i < index -1; ++i) {
        now = now->next;
    } //遍历n-1次 找到now就是插入/删除元素的前继
    return now;
}

template<typename T>
void LinkList<T>::Insert(ListNode<T> *p) {
    p->next = NULL; //因为插入在最后，所以强制把p->next转为NULL
    ListNode<T>* now = head;
    while (now->next != NULL) {
        now = now->next;
    }
    if (now == NULL) now = p;
    else { //now != next
        now->next = p;
    }
    currentLength++;
}

template<typename T>
void LinkList<T>::Insert(T q) {
    ListNode<T> *p = new ListNode<T>(q);
    p->next = NULL; //因为插入在最后，所以强制把p->next转为NULL
    ListNode<T>* now = head;
    while (now->next != NULL) {
        now = now->next;
    }
    if (now == NULL) now = p;
    else { //now != next
        now->next = p;
    }
    currentLength++;
}

template<typename T>
void LinkList<T>::Insert(ListNode<T> *p, int index) { //插入到第number个data的前面，所以number = 0 和 number = 1 都是插入在第一个
    if (index > currentLength) {Insert(p); return;}
    ListNode<T>* now = Find(index);
    ListNode<T>* tmp = now->next;
    now->next = p;
    p->next = tmp;
    currentLength++;
}

template<typename T>
void LinkList<T>::Delete() {
    if (currentLength == 0) return;
    ListNode<T>* now = Find(currentLength);
    delete(now->next);
    now->next = NULL;
    currentLength--;
}

template<typename T>
void LinkList<T>::Delete(int index) {
    if (currentLength == 0) return;
    //if (index > currentLength) {Delete(); return;}
    if (index > currentLength) return;
    ListNode<T>* now = Find(index);
    ListNode<T>* tmp = now->next; //要删除的元素
    now->next = tmp->next;
    delete(tmp);
    currentLength--;
}

template<typename T>
void LinkList<T>::Show() const{
    if (head == NULL) {
        qDebug() << "head = null" <<  Qt::endl;
        return;
    }
    ListNode<T>* now = head;
    while(now != NULL) {
        qDebug()  << "("<< now->data.x << "," << now->data.y << ")";
        now = now->next;
    }
    qDebug() <<  Qt::endl;
}

template<typename T>
void LinkList<T>::Show(int from, int to) const{
    ListNode<T>* now = head;
    for (int i = 0 ; i < from; i++) {
        now = now->next;
    }
    for (int i = 0; i <= (to - from); i++) {
        printf("%d\t%s\n", from + i, now->data.c_str());
        now = now->next;
    }
}

#endif // LINKLIST_H
