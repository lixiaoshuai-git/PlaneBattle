#include "plane.h"
#include "QPixmap"
#include "config.h"
#include "bullet.h"
Plane::Plane() {
    image.load(":/new/res/hero.png");
}

Plane::~Plane()
{
    foreach (Bullet* bullet, bullets) {
        delete bullet;
    }
    bullets.clear();
}
void Plane::draw(QPainter &painter)
{

    this->move();
    if (invincible) {
        // 无敌中每5帧闪烁一次
        if (invincibleTime % 10 < 5) {
            painter.drawPixmap(x, y, image);
        }
    } else {
    painter.drawPixmap(x, y, image); // 绘制飞机
    }
    // 计算屏幕右上方坐标（基于全局窗口尺寸WIN_W、WIN_H，来自config.h）
    int hpBarX = WIN_W - hpBarWidth - hpBarMargin; // 右边缘间距
    int hpBarY = hpBarMargin; // 上边缘间距

    // 绘制血条背景（灰色）
    painter.setPen(Qt::darkGray);
    painter.setBrush(Qt::darkGray);
    painter.drawRect(hpBarX, hpBarY, hpBarWidth, hpBarHeight);

    // 绘制当前血量（绿色，可根据剩余血量变色）
    QColor hpColor = Qt::green;
    if (hp < 30) hpColor = Qt::red; // 血量低于30%变红
    else if (hp < 60) hpColor = Qt::yellow; // 30%-60%变黄

    painter.setPen(hpColor);
    painter.setBrush(hpColor);
    int currentHpWidth = (hp * 1.0 / maxHp) * hpBarWidth; // 按比例计算当前血量宽度
    painter.drawRect(hpBarX, hpBarY, currentHpWidth, hpBarHeight);

    // 可选：绘制血量文本（如“100/100”）
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10));
    QString hpText = QString("HP:%1/%2").arg(hp).arg(maxHp);
    painter.drawText(hpBarX, hpBarY - 15, hpText); // 文本显示在血条上方



    if (isFire)
    fireCounter++;
    if (fireCounter >= fireDelay) { // 达到间隔才发射
    // 只在开火时创建新子弹
    Bullet* newBullet = new Bullet(firelevel);
    // 设置子弹初始位置（基于飞机位置居中）
    newBullet->bullet_x = x + image.width()/2 - newBullet->bullet.width()/2;
    newBullet->bullet_y = y;  // 子弹从飞机顶部发射
    bullets.append(newBullet);
    fireCounter = 0; // 重置计时器
    isFire = false;  // 重置开火状态，避免一次按键创建多个子弹
    }


    // 遍历所有子弹，更新位置并绘制
        for (int i = 0; i < bullets.size(); ) {
            Bullet* bullet = bullets[i];
            if (bullet) {
            // 更新子弹位置（向上移动，假设y减小为向前）
                bullet->bullet_pos();  // 控制移动速度
            // 绘制子弹
                bullet->draw(painter);
            // 移除超出屏幕的子弹（避免内存泄漏）
                if (bullet->bullet_y < 0) {
                delete bullet;
                bullets.removeAt(i);
                } else {
                    i++;  // 只有子弹未移除时才递增索引
                }
            }
        }
    }

void Plane::drawPlayer2(QPainter &painter)
{
    this->move();

        painter.drawPixmap(x, y, image);

    // 计算屏幕右上方坐标（基于全局窗口尺寸WIN_W、WIN_H，来自config.h）
    int hpBarX =  hpBarMargin; // 右边缘间距
    int hpBarY = hpBarMargin; // 上边缘间距

    // 绘制血条背景（灰色）
    painter.setPen(Qt::darkGray);
    painter.setBrush(Qt::darkGray);
    painter.drawRect(hpBarX, hpBarY, hpBarWidth, hpBarHeight);

    // 绘制当前血量（绿色，可根据剩余血量变色）
    QColor hpColor = Qt::green;
    if (hp < 30) hpColor = Qt::red; // 血量低于30%变红
    else if (hp < 60) hpColor = Qt::yellow; // 30%-60%变黄

    painter.setPen(hpColor);
    painter.setBrush(hpColor);
    int currentHpWidth = (hp * 1.0 / maxHp) * hpBarWidth; // 按比例计算当前血量宽度
    painter.drawRect(hpBarX, hpBarY, currentHpWidth, hpBarHeight);

    // 可选：绘制血量文本（如“100/100”）
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10));
    QString hpText = QString("HP:%1/%2").arg(hp).arg(maxHp);
    painter.drawText(hpBarX, hpBarY - 15, hpText); // 文本显示在血条上方


    if (isFire) {
        // 只在开火时创建新子弹（建议加个发射间隔控制）
        fireCounter2++;
        if (fireCounter2 >= fireDelay) { // 达到间隔才发射
            // 只在开火时创建新子弹（建议加个发射间隔控制）
        Bullet* newBullet = new Bullet(0);
        // 设置子弹初始位置（基于飞机位置居中）
        newBullet->bullet_x = x + image.width()/2 - newBullet->bullet.width()/2;
        newBullet->bullet_y = y;  // 子弹从飞机顶部发射
        bullets.append(newBullet);
        fireCounter2=0;
        isFire = false;  // 重置开火状态，避免一次按键创建多个子弹
        }
    }
    // 遍历所有子弹，更新位置并绘制
    for (int i = 0; i < bullets.size(); ) {
        Bullet* bullet = bullets[i];
        if (bullet) {
            // 更新子弹位置（向上移动，假设y减小为向前）
            bullet->bullet_pos();  // 控制移动速度
            // 绘制子弹
            bullet->draw(painter);
            // 移除超出屏幕的子弹（避免内存泄漏）
            if (bullet->bullet_y < 0) {
                delete bullet;
                bullets.removeAt(i);
            } else {
                i++;  // 只有子弹未移除时才递增索引
            }
        }
    }
}
    //bullet->draw(painter);

void Plane::move()
{
    if (invincibleTime > 0) {
        invincibleTime--;
    } else {
        invincible = false; // 时间到后关闭无敌
    }
    if(isUp && !isDown && !isLeft && !isRight){// 上
        if(y>40){
            y-=PLANE_MOVE_SPEED;
        }
    }else if(!isUp && isDown && !isLeft && !isRight){//下
        if(y<715)
        {
            y+=PLANE_MOVE_SPEED;
        }
    }else if(!isUp && !isDown && isLeft && !isRight){//左
        if(x>-35){
            x-=PLANE_MOVE_SPEED;
        }

    }else if(!isUp && !isDown && !isLeft && isRight){//右
        if(x<445){
            x+=PLANE_MOVE_SPEED;
        }
    }else if(isUp && !isDown && isLeft && !isRight){//左上
        if(x>-35&&y>40){
        x-=PLANE_MOVE_SPEED;
        y-=PLANE_MOVE_SPEED;
        }

        //x -35,445
        //    y 40,715

    }else if(!isUp && isDown && isLeft && !isRight){//左下
        if(x>-35&&y<715){
        x-=PLANE_MOVE_SPEED;
        y+=PLANE_MOVE_SPEED;
        }

    }else if(isUp && !isDown && !isLeft && isRight){//右上
        if(x<445&&y>40){
        x+=PLANE_MOVE_SPEED;
        y-=PLANE_MOVE_SPEED;
        }


    }else if(!isUp && isDown && !isLeft && isRight){//右下
        if(x<445&&y<715){
        x+=PLANE_MOVE_SPEED;
        y+=PLANE_MOVE_SPEED;
        }
    }
    rect.setWidth(image.width());
    rect.setHeight(image.height());
    rect.moveTo(x,y);
}
