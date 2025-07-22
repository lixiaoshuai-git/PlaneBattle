#ifndef BOSS_H
#define BOSS_H
#include <QPainter>
#include <QList>
#include "bossbullet.h"
#include "config.h"

class Boss {
public:
    Boss();
    void draw(QPainter& painter);
    void update(int frame);          // 每帧更新弹幕和位置
    void shoot();                    // 发射弹幕
    bool alive()  { return hp > 0; }  // 判断是否存活
    void takeDamage(int damage);     // 接收伤害
    int hp = 0;            // 当前血量
    int x = WIN_W/2 - 100;           // 初始X坐标（居中）
    int y = 100;                     // 初始Y坐标
    QPixmap img;                     //  boss图片
    QList<BossBullet*> bullets;      // 弹幕列表
    int phase() const {              // 根据血量判断阶段（1-3）
        return 3 - (hp - 1) / BOSS_PHASE_HP;
    }
};
#endif // BOSS_H
