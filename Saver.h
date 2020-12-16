#ifndef SAVER_H
#define SAVER_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include "LinkList.h"
#include <vector>
#include "Game.h"

class Saver{
private:
    Background background[3];
    LinkList<Snake> snakeList[3];
    bool saved[3];
    int saverNumber = 0;
    //Setting setting;
public:
    Saver();
    Saver(Game*);
    int getSaverNumber();
    bool getSaved(int);
    void setSaved(int, bool);
    void load(int, Game*);
    void save(int, Game*);
};
#endif // SAVER_H
