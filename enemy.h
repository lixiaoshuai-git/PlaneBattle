#ifndef ENEMY_H
#define ENEMY_H
#include <QPainter>
#include <QKeyEvent>
#include <QPixmap>
#include <qrect.h>
#include "enemybullet.h"
#include <QPainter>
class Enemy
{
public:
    QList<EnemyBullet*> bullets;
    int fireInterval; // 发射间隔计时器
    int fireDelay; // 发射间隔（如50帧）
public:
    Enemy();
    ~Enemy();
    void draw(QPainter &painter);
    int randint;
    QPixmap image;
    int x ;
    int y ;
    int hp;
    bool isFire = true;
    bool free = true;
    void move();
    void shoot();
    QRect rect;
    QPainter painter;
};

#endif // ENEMY_H
