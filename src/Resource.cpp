#include <QPainter>
#include "header/Resource.h"


void Resource::init() {
    character = new QPixmap[size];
    food = new QPixmap[size];

    character[0].load(":/image/image/head1.png");
    character[1].load(":/image/image/head2.png");
    character[2].load(":/image/image/head3.png");
    food[0].load(":/image/image/head4.png");
    food[1].load(":/image/image/head5.png");
    food[2].load(":/image/image/head6.png");
    food[3].load(":/image/image/head7.png");
    food[4].load(":/image/image/head8.png");
}

Resource::Resource(){
    init();
}
