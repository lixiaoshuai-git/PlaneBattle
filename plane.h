#ifndef PLANE_H
#define PLANE_H
#include <QPainter>
#include <QKeyEvent>
#include <QPixmap>
#include <qrect.h>
#include "bullet.h"
#include <QPainter>
class Plane
{
public:
    QList<Bullet*> bullets;  // 存储所有子弹
public:
    Plane();
    ~Plane();
    void draw(QPainter &painter);
    void drawPlayer2(QPainter &painter);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    QPixmap image;
    int x =200;
    int y =600;

    int hp = 100; // 初始血量
    int maxHp = 100; // 最大血量
    // 血条位置和大小
    int hpBarWidth = 120;   // 血条宽度
    int hpBarHeight = 10;   // 血条高度
    int hpBarMargin = 30;   // 距离屏幕边缘的间距（右、上）
    int invincibleTime = 0; // 无敌持续时间（帧）
    // 存储 战机移动 的键是否被按下
    bool isUp = false;
    bool isDown = false;
    bool isLeft = false;
    bool isRight = false;
    bool isFire = false;
    bool invincible = false;
    bool isAlive = true;
    void move();
    QRect rect;
    QPainter painter;
    int firelevel=0;
    int fireDelay = 10; // 发射间隔（帧），值越小射速越快
    int fireCounter = 0; // 发射计时器
    int fireCounter2 = 0;
    //全屏大炸弹
    int  superBomb = MAX_BOMB;   // 剩余炸弹
    bool useBomb   = false;      // 本帧是否触发
};

#endif // PLANE_H
