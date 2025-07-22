#ifndef PLAY03_H
#define PLAY03_H

#include <QPainter>
#include <QKeyEvent>
#include "plane.h"
#include "enemy.h"
#include "bomb.h"
#include "prop.h"
#include "boss.h"
#include "music.h"

class Play03
{
public:
    Play03();
    ~Play03();
    void draw(QPainter &painter);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void enemyToScene();
    void collisionDetection();
    void enemycollision(Plane * plane);
    void judgewhoWin();
    void createProp(int x, int y);
    void propCollisionDetection();

    Plane *plane1 = nullptr;   // 玩家1
    Plane *plane2 = nullptr;   // 玩家2

    Enemy  m_enemys[ENEMY_NUM];
    Bomb   m_bombs[BOMB_NUM];
    Prop   m_props[PROP_NUM];
    Boss*  boss = nullptr;
    bool   bossSpawned = false;
    int    m_recorder = 0;
    int    Count1 = 0;
    int    Count2 = 0;
    int    frame = 0;
    Music* music;
};

#endif // PLAY03_H
