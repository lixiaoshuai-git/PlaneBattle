#ifndef ENEMYBULLET_H
#define ENEMYBULLET_H
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include "config.h"

class EnemyBullet
{
public:
    EnemyBullet();

    // 更新子弹位置
    void move();
    // 绘制子弹
    void draw(QPainter &painter);

public:
    QPixmap bullet;       // 子弹图片
    int x;                // 子弹x坐标
    int y;                // 子弹y坐标
    int speed=BULLET_SPEED ;            // 子弹速度（向下发射，速度可与玩家子弹不同）
    bool is_free;         // 子弹是否空闲（未激活/超出屏幕）
    QRect rect;           // 子弹碰撞矩形
};

#endif // ENEMYBULLET_H
