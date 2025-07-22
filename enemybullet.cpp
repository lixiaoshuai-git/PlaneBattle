#include "enemybullet.h"
#include "widget.h"


EnemyBullet::EnemyBullet()
    : speed(7),is_free(true) // 敌机子弹速度稍慢于玩家子弹
{
    // 加载敌机子弹图片（根据实际资源路径调整）
    bullet.load(":/new/res/bullet_4.png");

    // 初始化碰撞矩形
    rect.setWidth(bullet.width());
    rect.setHeight(bullet.height()-10);
    rect.moveTo(x, y);
}
void EnemyBullet::move()
{
    if(Widget::isPaused == false){
        if (is_free) return;  // 空闲状态不移动
        // 敌机子弹向下移动（y递增）
        y += speed;
        rect.moveTo(x, y);
        // 超出屏幕底部则设为空闲
        if (y >= WIN_H) {
            is_free = true;
        }
    }
}
void EnemyBullet::draw(QPainter &painter)
{
    if (!is_free) {  // 非空闲状态才绘制
        painter.drawPixmap(x, y, bullet);
    }
}
