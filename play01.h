#ifndef Play01_H
#define Play01_H
#include <QPainter>
#include <QKeyEvent>
#include <QPixmap>
#include"plane.h"
#include"enemy.h"
#include"bomb.h"
#include "prop.h"
#include "boss2.h"
#include "boss.h"
#include"music.h"


class Play01
{
public:
    Play01();
    ~Play01();
    //   处理画笔的函数
    void draw(QPainter &painter);
    //    处理键盘事件的函数
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    QPixmap enter;
    QRect enterRect;
    Plane* plane=nullptr;
    //敌机出场
    void enemyToScene();
    void gamelevel();
    int counter=0;
    //敌机数组
    Enemy m_enemys[ENEMY_NUM];
    Bomb m_bombs[BOMB_NUM];
    //敌机出场间隔记录
    int m_recorder=0;
    void collisionDetection();
    int Count=0;

    //道具数组
    Prop m_props[PROP_NUM];
    void createProp(int x, int y);
    void propCollisionDetection();
    //boss
    Boss* boss = nullptr;       // BOSS对象
    bool bossSpawned = false;   // 是否已生成BOSS
    int frame=0;
    Music * music;
    int enemyintereval=ENEMY_INTERVAL;
    //精英Boss
    Boss2 * boss2 = nullptr;
    bool boss2Swpawned = false;
    int frame2 =0;
};

#endif // Play01_
