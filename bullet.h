#ifndef BULLET_H
#define BULLET_H
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QKeyEvent>
#include "config.h"
class Bullet
{
public:
    Bullet(int n);
    QPixmap bullet;
    void bullet_pos();
    int level;
    int bullet_x;
    int bullet_y;
    int bullet_speed=BULLET_SPEED;
    bool is_free=false;
    QRect rect;
    void draw(QPainter& painter);
    void reloadbullet();
};

#endif // BULLET_H
