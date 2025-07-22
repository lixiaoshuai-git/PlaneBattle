#ifndef PROP_H
#define PROP_H
#include <QPixmap>
#include <QRect>
#include "config.h"
class Prop {
public:
    Prop();

    // 绘制道具
    void draw(QPainter &painter);
    // 道具移动（向下掉落）
    void move();
    // 道具图片
    QPixmap img;
    // 位置坐标
    int x;
    int y;
    // 移动速度
    int speed;
    // 是否空闲（未被拾取）
    bool free;
    // 道具类型
    int type;
    // 碰撞矩形
    QRect rect;
};

#endif // PROP_H
