#ifndef Play02_H
#define Play02_H
#include <QPainter>
#include <QKeyEvent>
#include <QPixmap>
#include"plane.h"
#include"enemy.h"
#include"bossbullet.h"
#include "prop.h"
#include"music.h"
#include"bomb.h"
class Play02
{
public:
    Play02();
    ~Play02();
    //   处理画笔的函数
    void draw(QPainter &painter);
    //    处理键盘事件的函数
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void createProp(int x, int y);
    void propCollisionDetection();

    Plane* plane1=nullptr;
    Plane* plane2=nullptr;
    //敌机出场
    void enemyToScene();
    int Count= 0;
    //敌机数组
    Enemy m_enemys[ENEMY_NUM];
    Bomb m_bombs[BOMB_NUM];
    //敌机出场间隔记录
    int m_recorder=0;
    void collisionDetection();

    Prop   m_props[PROP_NUM];
    Music* music;

    int hp = 100; // 初始血量
    int maxHp = 100; // 最大血量
    // 血条位置和大小
    int hpBarWidth = 120;   // 血条宽度
    int hpBarHeight = 10;   // 血条高度
    int hpBarMargin = 30;   // 距离屏幕边缘的间距（右、上）
};

#endif
