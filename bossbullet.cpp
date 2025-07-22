#include "bossbullet.h"

BossBullet::BossBullet(qreal _x, qreal _y, qreal _dx, qreal _dy)
    : x(_x), y(_y), dx(_dx), dy(_dy) {
    img.load(":/new/res/bullet_1.png"); // 加载弹幕图片
    rect.setWidth(img.width()-2);
    rect.setHeight(img.height()-2);
    rect.moveTo(x,y);
}

void BossBullet::update() {
    x += dx;
    y += dy;
    rect.moveTo(x, y); // 同步更新碰撞矩形位置
}

void BossBullet::draw(QPainter& p) {
    p.drawPixmap(x, y, img);  // 绘制弹幕
}

bool BossBullet::out() const {
    // 超出屏幕范围判断
    return x < 0 || x > WIN_W || y < 0 || y > WIN_H;
}


