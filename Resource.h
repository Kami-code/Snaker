#pragma once

#ifndef RESOURCE_H
#define RESOURCE_H

#include <QPainter>

class Resource{
public:
    QPixmap* character,* food;
    int size = 50;

    void init();
    Resource();
};


#endif // RESOURCE_H
