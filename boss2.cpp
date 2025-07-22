#include "boss2.h"
#include <QDebug>
#include <cmath>
#include <QRandomGenerator>

// 弹幕类型常量
const int BULLET_TYPE_NORMAL = 1;    // 普通弹幕
const int BULLET_TYPE_HOMING = 2;    // 追踪弹幕

Boss2::Boss2() {
    if (!img.load(":/new/res/img-plane_7.png")) {
        qDebug() << "BOSS图片加载失败！";
    }
    hp = BOSS_MAX_HP2;
    a = 0;
    bulletPatternIndex = 0;
    fireRate = 60;         // 初始射速（帧）
    moveDirX = 0;
    moveDirY = 0;
}

void Boss2::draw(QPainter& p) {
    if (!alive()) return;

    // 绘制BOSS本体
    p.drawPixmap(m_x, m_y, img);

    // 绘制血条
    int bw = 400, bh = 12;
    int bx = (WIN_W - bw) / 2;
    int by = 40;
    p.setBrush(Qt::gray);
    p.drawRect(bx, by, bw, bh);

    p.setBrush(Qt::red);
    p.drawRect(bx, by, static_cast<int>(bw * 1.0 * hp / BOSS_MAX_HP2), bh);

    // 绘制所有弹幕
    for (auto bullet : bullets) {
        bullet->draw(p);
    }
}

void Boss2::update(int frame, int playerX, int playerY) {
    if (!alive()) return;

    // 根据阶段阶段调整射速
    move();
    adjustFireRateByPhase();

    // 按帧发射弹幕
    if (frame % fireRate == 0) {
        shoot(playerX, playerY);
        bulletPatternIndex = (bulletPatternIndex + 1) % getPatternCountByPhase();
    }

    // 更新所有弹幕
    for (auto it = bullets.begin(); it != bullets.end();) {
        // 对追踪弹幕特殊处理
        if ((*it)->type() == BULLET_TYPE_HOMING) {
            (*it)->homeTo(playerX, playerY);
        }
        (*it)->update();

        ++it;

    }
    // 修复建议：在子弹更新循环中添加移除逻辑
    for (auto it = bullets.begin(); it != bullets.end();) {
        if ((*it)->type() == BULLET_TYPE_HOMING) {
            (*it)->homeTo(playerX, playerY);
        }
        (*it)->update();

        if ((*it)->out()) { // 假设 BossBullet2 有 out() 方法判断是否出界
            delete *it;
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
}

// 根据当前阶段调整射速
void Boss2::adjustFireRateByPhase() {
    int ph = phase();
    fireRate = (ph == 1) ? 60 : 40; // 阶段1:60帧 阶段2:40帧
}

// 根据阶段获取弹幕模式数量
int Boss2::getPatternCountByPhase() {
    return (phase() == 1) ? 2 :2;  // 阶段1:2种模式 阶段2:2种模式
}

void Boss2::shoot(int playerX, int playerY) {
    int ph = phase();
    int centerX = x() + img.width() / 2;
    int centerY = y() + img.height() / 2;

    // 根据当前阶段和模式索引选择弹幕模式
    if (ph == 1) {
        shootPhase1(centerX, centerY, playerX, playerY);
    } else {
        shootPhase2(centerX, centerY, playerX, playerY);
    }
}

// 阶段1弹幕模式：基础攻击（血量>50%）
void Boss2::shootPhase1(int cx, int cy, int px, int py) {
    switch(bulletPatternIndex) {
    // 模式1：三向直线弹幕
    case 0:
        for (int i = -1; i <= 1; ++i) {
            bullets.append(new BossBullet2(cx, cy, i * 30+90, BOSS_BULLET_SPD, BULLET_TYPE_NORMAL));
        }
        break;

    // 模式2：V形弹幕
    case 1:
        bullets.append(new BossBullet2(cx, cy, -3+90, BOSS_BULLET_SPD + 1, BULLET_TYPE_NORMAL));
        bullets.append(new BossBullet2(cx, cy, 90, BOSS_BULLET_SPD, BULLET_TYPE_NORMAL));
        bullets.append(new BossBullet2(cx, cy, 93, BOSS_BULLET_SPD + 1, BULLET_TYPE_NORMAL));
        break;
    }
}

// 阶段2弹幕模式：强化攻击（血量≤50%）
void Boss2::shootPhase2(int cx, int cy, int px, int py) {
    switch(bulletPatternIndex) {
    // 模式1：散射弹幕
    case 0:
        for (int i = -2; i <= 2; ++i) { // 改为5个方向
            bullets.append(new BossBullet2(cx, cy, i * 30+90, BOSS_BULLET_SPD + 2, BULLET_TYPE_NORMAL));
        }
        break;

    // 模式2：追踪弹幕
    case 1:
        // 计算玩家方向的追踪弹幕
        double dx = px - cx;
        double dy = py - cy;
        double angle = atan2(dy, dx) * 180 / M_PI;
        bullets.append(new BossBullet2(cx, cy, angle, BOSS_BULLET_SPD - 1, BULLET_TYPE_HOMING));
        bullets.append(new BossBullet2(cx, cy, angle + 10, BOSS_BULLET_SPD - 1, BULLET_TYPE_HOMING));
        bullets.append(new BossBullet2(cx, cy, angle - 10, BOSS_BULLET_SPD - 1, BULLET_TYPE_HOMING));
        break;
}
}
// 创建爆炸效果
void Boss2::createExplosionEffect(int x, int y) {
    // 爆炸时产生8向碎片
    for (int i = 0; i < 360; i += 45) {
        bullets.append(new BossBullet2(x, y, i, 2, BULLET_TYPE_NORMAL));
    }
}

void Boss2::takeDamage(int damage) {
    if (!alive()) return;
    hp -= damage;
    if (hp < 0) hp = 0;
}

// 获取当前阶段（1-2阶段，以50%血量为界）
int Boss2::phase() {
    return (hp > BOSS_MAX_HP2 * 0.5) ? 1 : 2;
}

void Boss2::move() {
    int ph = phase();
    a++;
    double rad = qDegreesToRadians(static_cast<double>(a));

    // 阶段不同，移动模式不同
    if (ph == 1) {
        // 阶段1：左右平稳摆动
        m_x += cos(rad) * 5;
    } else {
        // 阶段2：快速随机移动
        if (a % 60 == 0) {
            // 每60帧改变一次移动方向
            moveDirX = QRandomGenerator::global()->bounded(3) - 1; // -1, 0, 1
            moveDirY = QRandomGenerator::global()->bounded(3) - 1;
        }
        m_x += moveDirX * 2;
        m_y += moveDirY * 2;

        // 限制在屏幕内
        if (x() < 0) m_x = 0;
        if (x() > WIN_W - img.width()) m_x = WIN_W - img.width();
        if (y() < 50) m_y = 50;
        if (y() > WIN_H / 3) m_y = WIN_H / 3;
    }
}
