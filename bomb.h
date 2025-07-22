#ifndef BOMB_H
#define BOMB_H
#include "config.h"
#include <QPixmap>
#include <QVector>

class Bomb {
public:
    Bomb();
    void draw(QPainter& painter);
    void updateInfo(); // 更新爆炸帧状态

    // 爆炸位置
    int x;
    int y;
    // 爆炸状态（是否空闲）
    bool free;
    // 爆炸帧索引（控制动画播放）
    int currentFrame;
    // 爆炸帧计数器（控制帧切换速度）
    int frameCounter;
    // 存储所有爆炸帧图片
    QVector<QPixmap> frames;

private:
    // 初始化爆炸帧资源
    void extracted(QString &path);
    void initFrames();
};

#endif // BOMB_H
