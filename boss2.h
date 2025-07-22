#ifndef BOSS2_H
#define BOSS2_H
#include <QPainter>
#include <QList>
#include "bossbullet2.h"
#include "config.h"

class Boss2 {
public:
    Boss2();
    // 绘制BOSS和弹幕
    void draw(QPainter &p);

    // 更新BOSS状态和弹幕（需要玩家位置用于追踪弹幕追踪）
    void update(int frame, int playerX, int playerY);

    // 发射弹幕
    void shoot(int playerX, int playerY);

    // 受到伤害
    void takeDamage(int damage);

    // 移动逻辑
    void move();

    // 判断是否存活
    bool alive() const { return hp > 0; }

    // 获取当前阶段（1或2）
    int phase();
    // 获取位置和尺寸
    int x() const { return m_x; }
    int y() const { return m_y; }
    int width() const { return img.width(); }
    int height() const { return img.height(); }

    // 设置位置
    void setPos(int x, int y) { m_x = x; m_y = y; }

    // 获取所有子弹（用于碰撞检测）
    QVector<BossBullet2*> &getBullets() { return bullets; }

public:
    // 阶段1弹幕模式
    void shootPhase1(int cx, int cy, int px, int py);

    // 阶段2弹幕模式
    void shootPhase2(int cx, int cy, int px, int py);

    // 根据阶段调整射速
    void adjustFireRateByPhase();

    // 根据阶段获取弹幕模式数量
    int getPatternCountByPhase();
    // 创建爆炸效果
    void createExplosionEffect(int x, int y);
    QPixmap img;               // BOSS图片
    int hp;                    // 当前生命值
    int m_x = WIN_W/2 - 50;           // 初始X坐标（居中）
    int m_y = 100;                     // 初始Y坐标
    int a;                     // 用于移动计算的计数器
    int bulletPatternIndex;    // 弹幕模式索引
    int fireRate;              // 射速（帧）
    int moveDirX, moveDirY;    // 移动方向
    QVector<BossBullet2*> bullets;  // 弹幕容器
};
#endif // BOSS2_H
