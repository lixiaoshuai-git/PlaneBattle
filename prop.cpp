#include "prop.h"
#include <QPainter>
#include <cstdlib>

Prop::Prop() : free(true), speed(3) {
    // 随机初始化道具类型
    type = rand() % 3;
    // 根据类型加载对应图片（需提前准备资源）
    switch (type) {
    case PROP_HP_ADD:
        img.load(":/new/res/tool1.png");
        break;
    case PROP_FIRE_UP:
        img.load(":/new/res/tool2.png");
        break;
    case PROP_INVINCIBLE:
        img.load(":/new/res/tool3.png");
        break;
    }
    // 初始化碰撞矩形
    rect.setWidth(img.width());
    rect.setHeight(img.height());
}

void Prop::draw(QPainter &painter) {
    if (!free) {
        painter.drawPixmap(x, y, img);
    }
}

void Prop::move() {
    if (free) return;
    y += speed;
    rect.moveTo(x, y);
    // 超出屏幕底部则设为空闲
    if (y >= WIN_H) {
        free = true;
    }
}
