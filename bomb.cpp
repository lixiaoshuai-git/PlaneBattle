#include "bomb.h"
#include <QPainter>
#include <qdebug.h>

Bomb::Bomb() : free(true), currentFrame(0), frameCounter(0) {
    initFrames(); // 加载爆炸动画帧
}

// 初始化爆炸帧资源（从配置文件定义的路径加载）
void Bomb::initFrames() {
    // 加载BOMB_MAX+1张爆炸图片（索引0到BOMB_MAX）
    for (int i = 1; i <= BOMB_MAX; ++i) {
        QString path = QString(":/new/res/bomb-%1.png").arg(i);
        QPixmap frame;
        if (frame.load(path)) {
            frames.push_back(frame);
        } else {
            // 加载失败时用空白图替代，避免崩溃
            frames.push_back(QPixmap(100, 100));
            qDebug() << "爆炸图片加载失败：";
        }
    }
}

// 绘制当前爆炸帧
void Bomb::draw(QPainter& painter) {
    if (free) return; // 空闲状态不绘制

    // 绘制当前帧图片
    painter.drawPixmap(x, y, frames[currentFrame]);
}

// 更新爆炸动画状态（切换帧）
void Bomb::updateInfo() {
    if (free) return; // 空闲状态不更新

    frameCounter++;
    // 达到帧间隔时切换到下一帧
    if (frameCounter >= BOMB_INTERVAL) {
        frameCounter = 0; // 重置计数器
        currentFrame++;   // 切换到下一帧

        // 动画播放完毕，回到空闲状态
        if (currentFrame >= frames.size()) {
            free = true;
            currentFrame = 0; // 重置帧索引，为下次爆炸做准备
        }
    }
}
