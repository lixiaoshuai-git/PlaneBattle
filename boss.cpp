#include "boss.h"
#include <QDebug>

Boss::Boss() {
    if (!img.load(":/new/res/boss.png")) {
        qDebug() << "BOSS图片加载失败！"; // 错误提示
    }
}

void Boss::draw(QPainter& p) {
    if (!alive()) return; // 死亡后不绘制

    // 绘制BOSS本体
    p.drawPixmap(x, y, img);

    // 绘制血条
    int bw = 400, bh = 12;
    int bx = (WIN_W - bw) / 2; // 水平居中
    int by = 40;               // 顶部位置
    p.setBrush(Qt::gray);
    p.drawRect(bx, by, bw, bh); // 血条背景

    p.setBrush(Qt::red);
    // 按剩余血量绘制当前血条
    p.drawRect(bx, by, static_cast<int>(bw * 1.0 * hp / BOSS_MAX_HP), bh);

    // 绘制所有弹幕
    for (auto bullet : bullets) {
        bullet->draw(p);
    }
}

void Boss::update(int frame) {
    if (!alive()) return;

    // 按帧发射弹幕（每60帧一次）
    if (frame % 60 == 0) {
        shoot();
    }

    // 更新所有弹幕位置，移除超出屏幕的弹幕
    for (auto it = bullets.begin(); it != bullets.end();) {
        (*it)->update();
        if ((*it)->out()) {
            delete *it;    // 释放内存
            it = bullets.erase(it); // 从列表移除
        } else {
            ++it;
        }
    }
}

void Boss::shoot() {
    int ph = phase(); // 获取当前阶段
    int centerX = x + img.width() / 2; // BOSS中心X坐标
    int centerY = y + img.height() / 2; // BOSS中心Y坐标

    // 阶段1：直线三发弹幕
    if (ph == 1) {
        for (int i = -1; i <= 1; ++i) {
            bullets.append(new BossBullet(centerX, centerY, i * 2, BOSS_BULLET_SPD));
        }
    }
    // 阶段2：环形弹幕（12个方向）
    else if (ph == 2) {
        for (int a = 0; a < 360; a += 30) {
            double rad = qDegreesToRadians(static_cast<double>(a));
            bullets.append(new BossBullet(centerX, centerY,
                                          BOSS_BULLET_SPD * cos(rad),
                                          BOSS_BULLET_SPD * sin(rad)));
        }
    }
    // 阶段3：散射弹幕（加速）
    else {
        for (int i = -2; i <= 2; ++i) {
            bullets.append(new BossBullet(centerX, centerY,
                                          i * 3, BOSS_BULLET_SPD + 2));
        }
    }
}

void Boss::takeDamage(int damage) {
    if (!alive()) return;
    hp -= damage;
    if (hp < 0) hp = 0; // 血量不低于0
}
