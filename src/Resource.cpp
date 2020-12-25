#include <iostream>
#include <cstring>
#include <QPainter>
#include "header/Resource.h"

extern Game game;

void Resource::init() {
    int tmp = min(12, maxSnake);
    character = new QPixmap[tmp];
    food = new QPixmap[tmp];
    for (int i = 0; i < 12; ++i) {
        QString s = ":/image/image/head" + QString::number(i) + ".png";
        character[i].load(s);
    }
    for (int i = 0; i < 5; ++i) {
        QString s = ":/image/image/food" + QString::number(i) + ".png";
        food[i].load(s);
    }
}

Resource::Resource(){
    init();
}
