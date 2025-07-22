#ifndef BOSSBULLET2_H
#define BOSSBULLET2_H
#include <QPainter>
#include "config.h"
#include <QRect>
#include <QObject>
#include <QDebug>
class BossBullet2 : public QObject
{
    Q_OBJECT
public:
    // 构造函数（x,y:位置；angle:角度；speed:速度；type:类型）
    BossBullet2(int x, int y, double angle, int speed, int type = 1, QObject *parent = nullptr);

    // 绘制弹幕
    void draw(QPainter &p);

    // 更新位置
    void update();

    // 追踪目标
    void homeTo(int targetX, int targetY);

    // 判断是否超出屏幕
    bool out() const;

    // 获取类型
    int type() const { return m_type; }
    QRect rect;
    // 获取位置
    int x() const { return m_x; }
    int y() const { return m_y; }
    int width() const { return img.width(); }
    int height() const { return img.height(); }

public:
    QPixmap img;       // 弹幕图片
    int m_x, m_y;      // 位置
    double angle;      // 角度（方向）
    int speed;         // 速度
    int m_type;        // 类型（1:普通 2:追踪 3:爆炸）
};
#endif // BOSSBULLET2_H
