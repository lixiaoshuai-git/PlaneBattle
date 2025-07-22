#include "bullet.h"

Bullet::Bullet(int n) {
    level=n;
    reloadbullet();
    rect.setWidth(bullet.width()-2);
    rect.setHeight(bullet.height()-2);
    rect.moveTo(bullet_x,bullet_y);
}
void Bullet::bullet_pos()
{
    if(is_free){
        return;
    }
    else{
        bullet_y-=bullet_speed+2*level;
        rect.moveTo(bullet_x,bullet_y);
    }
}
void Bullet::draw(QPainter& painter)
{
    if(!is_free){
    painter.drawPixmap(bullet_x,bullet_y,bullet);
    }
}
void Bullet::reloadbullet(){
    bullet.load(":/new/res/bullet_"+QString::number(9+level)+".png");
}

