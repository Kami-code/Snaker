#include <iostream>
#include <cstring>
#include <QPainter>
#include "header/Resource.h"

extern Game game;

int Resource::GetFoodSize() {return foodSize;}
int Resource::GetCharacterSize() {return characterSize;}
void Resource::SetFoodSize(int number) {foodSize = number;}
void Resource::SetCharacterSize(int number) {characterSize = number;}

void Resource::LoadResourceFromExternal() {

    character = new QPixmap[characterSize];
    food = new QPixmap[foodSize];
    for (int i = 0; i < characterSize; ++i) {
        QString s = ":/image/image/head" + QString::number(i) + ".png";
        character[i].load(s);
    }
    for (int i = 0; i < foodSize; ++i) {
        QString s = ":/image/image/food" + QString::number(i) + ".png";
        food[i].load(s);
    }
    wall.load(":/image/image/wall.png");
}

Resource::Resource(){
    SetFoodSize(5);
    SetCharacterSize(12);
    LoadResourceFromExternal();
}
