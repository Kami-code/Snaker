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
    int GetSaverNumber();
    bool GetSaved(int);
    void SetSaved(int, bool);
    void Load(int, Game*);
    void Save(int, Game*);
};
#endif // SAVER_H
