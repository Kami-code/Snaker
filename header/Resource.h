#pragma once

#ifndef RESOURCE_H
#define RESOURCE_H

#include <QPainter>
#include "header/Game.h"

class Resource{
public:
    QPixmap* character,* food;
    void init();
    Resource();
};


#endif // RESOURCE_H
