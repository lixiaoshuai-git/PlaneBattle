#ifndef BOSSBULLET_H
#define BOSSBULLET_H
#include <QPainter>
#include "config.h"
#include <QRect>

class BossBullet {
public:
    BossBullet(qreal _x, qreal _y, qreal _dx, qreal _dy);
    void update();                   // 更新位置
    void draw(QPainter& p);          // 绘制弹幕
    bool out() const;                // 判断是否超出屏幕
    QRect rect ;             // 碰撞矩形

    qreal x, y;                      // 位置
    qreal dx, dy;                    // 移动向量（x和y方向速度）
    QPixmap img;                     // 弹幕图片
};
#endif // BOSSBULLET_H
