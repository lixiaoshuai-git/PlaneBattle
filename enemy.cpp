#include "enemy.h"
#include "widget.h"
#include <ctime>

Enemy::Enemy():free(true), fireInterval(0), fireDelay(50),hp(100){

    randint = std::rand() % 7 + 1; // 生成1-7的随机数
    image.load(":/new/res/img-plane_"+QString::number(randint)+".png");
    rect.setWidth(image.width());
    rect.setHeight(image.height());
    rect.moveTo(x,y);
}

Enemy::~Enemy()
{
    // 释放子弹内存
    foreach (EnemyBullet* bullet, bullets) {
        delete bullet;
    }
    bullets.clear();
}
void Enemy::shoot() {
    if (Widget::isPaused == false){
        if (free) return; // 空闲敌机不发射

        fireInterval++;
        if (fireInterval < fireDelay) return; // 未到发射间隔
        fireInterval = 0;

        // 创建子弹并设置初始位置（敌机底部居中）
        EnemyBullet* newBullet = new EnemyBullet();
        newBullet->x = x + image.width()/2 - newBullet->rect.width()/2;
        newBullet->y = y + image.height(); // 从敌机底部发射
        newBullet->is_free = false;
        bullets.append(newBullet);
    }
}

void Enemy::draw(QPainter &painter)
{

    this->move();
    painter.drawPixmap(x,y,image);
    for (int i = 0; i < bullets.size(); ) {
        EnemyBullet* bullet = bullets[i];
        if (bullet->is_free) {
            delete bullet;
            bullets.removeAt(i);
        } else {
            bullet->move();
            bullet->draw(painter);
            i++;
        }
    }
}

void Enemy::move()
{
    if (Widget::isPaused == false){
        if(free)
        {
            return;
        }

        y += PLANE_MOVE_SPEED-2;
        rect.moveTo(x,y);

        if(y >= WIN_H)
        {
            free = true;
        }
    }
}
