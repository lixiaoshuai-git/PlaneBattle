#include "bossbullet2.h"
BossBullet2::BossBullet2(int x, int y, double angle, int speed, int type, QObject *parent)
    : QObject(parent), angle(angle), speed(speed), m_type(type)
{
    // 根据弹幕类型加载不同图片
    QString imgPath;
    switch(m_type) {
    case 2:  // 追踪弹幕
        imgPath = ":/new/res/bullet_6.png";
        break;
    default: // 普通弹幕
        imgPath = ":/new/res/bullet_7.png";
    }

    if (!img.load(imgPath)) {
        qDebug() << "弹幕图片加载失败：" << imgPath;
    }

    // 计算初始位置（使子弹从BOSS中心发射）
    m_x = x - img.width() / 2;
    m_y = y - img.height() / 2;
    rect.setWidth(img.width()-2);
    rect.setHeight(img.height()-2);
    rect.moveTo(m_x,m_y);
}

void BossBullet2::draw(QPainter &p) {
    p.drawPixmap(m_x, m_y, img);
}

void BossBullet2::update() {
    // 将角度转换为弧度
    double rad = qDegreesToRadians(angle);

    // 根据角度和速度计算位移
    m_x += cos(rad) * speed;
    m_y += sin(rad) * speed;
    rect.moveTo(m_x,m_y);
}

void BossBullet2::homeTo(int targetX, int targetY) {
    // 只有追踪弹幕需要执行此逻辑
    if (m_type != 2) return;

    // 计算子弹到目标的方向
    int dx = targetX - (m_x + img.width() / 2);
    int dy = targetY - (m_y + img.height() / 2);

    // 计算新角度（朝向目标）
    double newAngle = qRadiansToDegrees(atan2(dy, dx));

    // 平滑转向（每次最多转5度，使追踪更自然）
    double angleDiff = newAngle - angle;

    // 处理角度环绕（360度到0度的过渡）
    if (angleDiff > 180) angleDiff -= 360;
    if (angleDiff < -180) angleDiff += 360;

    // 限制转向速度
    if (angleDiff > 5) angle += 5;
    else if (angleDiff < -5) angle -= 5;
    else angle = newAngle;
}

bool BossBullet2::out() const {
    // 判断子弹是否超出屏幕范围（留出一定缓冲区域）
    return (m_x < -img.width() * 2) ||
           (m_x > WIN_W + img.width() * 2) ||
           (m_y < -img.height() * 2) ||
           (m_y > WIN_H + img.height() * 2);
}
