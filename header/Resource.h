#pragma once

#ifndef RESOURCE_H
#define RESOURCE_H

#include <QPainter>
#include "header/Game.h"

class Resource{
private:
    int characterSize = 10;
    int foodSize = 5;
public:
    QPixmap* character,* food, wall;
    Resource();
    void SetCharacterSize(int);
    void SetFoodSize(int);
    int GetCharacterSize();
    int GetFoodSize();
    void LoadResourceFromExternal();
};


#endif // RESOURCE_H
