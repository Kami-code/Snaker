#ifndef SAVER_H
#define SAVER_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include "LinkList.h"
#include <vector>
#include "Game.h"

class Saver{
    Background background;
    LinkList<Snake*> snakeList;
    //Setting setting;
    Saver(Game);
};
#endif // SAVER_H
