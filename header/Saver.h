#ifndef SAVER_H
#define SAVER_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include "Game.h"
#include "LinkList.h"


class Saver{
private:
    Game savedGame[3];
    bool saved[3];
    int saverNumber = 0;
public:
    Saver();
    int GetSaverNumber();
    bool GetSaved(int);
    void SetSaved(int, bool);
    void Load(int, Game*);
    void Save(int, Game*);
};
#endif // SAVER_H
