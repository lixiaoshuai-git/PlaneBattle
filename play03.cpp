#include "play03.h"
#include "config.h"
#include "widget.h"
#include <QDebug>

Play03::Play03()
{
    plane1 = new Plane();
    plane2 = new Plane();
    boss = new Boss();
    music = new Music();
    music->playbackMusic(music->bgMusic, music->bgv);
    // 初始位置区分
    plane1->x = 150;
    plane1->y = 600;
    plane1->hp = plane1->maxHp = 100;

    plane2->x = 300;
    plane2->y = 600;
    plane2->hp = plane2->maxHp = 100;
    plane2->image.load(":/new/res/hero2.png");
}

Play03::~Play03()
{
    delete plane1;
    delete plane2;
    delete boss;
    delete music;
}

void Play03::draw(QPainter &painter)
{
    plane2->draw(painter);
    plane1->drawPlayer2(painter);
    // 大炸弹剩余数量
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 15));
    QString bombText1 = QString("剩余大炸弹:%1").arg(plane1->superBomb);
    painter.drawText(plane1->hpBarMargin, plane1->hpBarMargin + 65,bombText1);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 15));
    QString bombText2 = QString("剩余大炸弹:%1").arg(plane2->superBomb);
    painter.drawText(WIN_W - plane2->hpBarWidth - plane2->hpBarMargin, plane2->hpBarMargin + 65,bombText2);

    // 中立敌机
    for (int i = 0; i < ENEMY_NUM; ++i) {
        if (!m_enemys[i].free) {
            m_enemys[i].shoot();
            m_enemys[i].draw(painter);
        }
    }

    // 爆炸
    for (int i = 0; i < BOMB_NUM; ++i) {
        if (!m_bombs[i].free) {
            m_bombs[i].draw(painter);
        }
    }

    // 道具
    for (int i = 0; i < PROP_NUM; ++i) {
        if (!m_props[i].free) {
            m_props[i].move();
            m_props[i].draw(painter);
        }
    }

    if (bossSpawned && boss->alive()) {
        boss->draw(painter);
        frame++;
        boss->update(frame);
    }
}

void Play03::keyPressEvent(QKeyEvent *event)
{
    int code = event->key();

    // ===== 玩家1 =====
    if (code == Qt::Key_W) plane1->isUp    = true;
    if (code == Qt::Key_S) plane1->isDown  = true;
    if (code == Qt::Key_A) plane1->isLeft  = true;
    if (code == Qt::Key_D) plane1->isRight = true;
    if (code == Qt::Key_E) {
        plane1->isFire = true;
        music->playshootEffect();
    }
    if (code == Qt::Key_Q) {
        if (plane1->superBomb > 0) plane1->useBomb = true;
    }

    // ===== 玩家2 =====
    if (code == Qt::Key_8)    plane2->isUp    = true;
    if (code == Qt::Key_5)  plane2->isDown  = true;
    if (code == Qt::Key_4)  plane2->isLeft  = true;
    if (code == Qt::Key_6) plane2->isRight = true;
    if (code == Qt::Key_Enter) {
        plane2->isFire = true;
        music->playshootEffect();
    }
    if (code == Qt::Key_Return) {
        if (plane2->superBomb > 0) plane2->useBomb = true;
    }
}

void Play03::keyReleaseEvent(QKeyEvent *event)
{
    int code = event->key();
    // ===== 玩家1 =====
    if (code == Qt::Key_W) plane1->isUp    = false;
    if (code == Qt::Key_S) plane1->isDown  = false;
    if (code == Qt::Key_A) plane1->isLeft  = false;
    if (code == Qt::Key_D) plane1->isRight = false;
    if (code == Qt::Key_Q) {
        if (plane1->superBomb > 0) plane1->useBomb = false;
    }

    // ===== 玩家2 =====
    if (code == Qt::Key_8)    plane2->isUp    = false;
    if (code == Qt::Key_5)  plane2->isDown  = false;
    if (code == Qt::Key_4)  plane2->isLeft  = false;
    if (code == Qt::Key_6) plane2->isRight = false;
    if (code == Qt::Key_Return) {
        if (plane2->superBomb > 0) plane2->useBomb = false;
    }
}

void Play03::enemyToScene()
{
    m_recorder++;
    if (m_recorder < ENEMY_INTERVAL) return;
    m_recorder = 0;

    for (int i = 0; i < ENEMY_NUM; ++i) {
        if (m_enemys[i].free) {
            m_enemys[i].free = false;
            m_enemys[i].x = rand() % (WIN_W - m_enemys[i].rect.width());
            m_enemys[i].y = -m_enemys[i].rect.height();
            break;
        }
    }
}

void Play03::collisionDetection()
{
    // 1. 玩家子弹打玩家2
    for (auto *b : plane1->bullets) {
        if (b->is_free) continue;
        if (plane2->rect.intersects(b->rect)) {
            b->is_free = true;
            plane2->hp -= 10;
            // 爆炸
            for (int k = 0; k < BOMB_NUM; ++k) {
                if (m_bombs[k].free) {
                    m_bombs[k].free = false;
                    m_bombs[k].x = plane2->x;
                    m_bombs[k].y = plane2->y;
                    break;
                }
            }
        }
    }

    // 2. 玩家2子弹打玩家1
    for (auto *b : plane2->bullets) {
        if (b->is_free) continue;
        if (plane1->rect.intersects(b->rect)) {
            b->is_free = true;
            plane1->hp -= 10;
            for (int k = 0; k < BOMB_NUM; ++k) {
                if (m_bombs[k].free) {
                    m_bombs[k].free = false;
                    m_bombs[k].x = plane1->x;
                    m_bombs[k].y = plane1->y;
                    break;
                }
            }
        }
    }

    // 3. 中立敌机子弹打双方
    enemycollision(plane1);
    enemycollision(plane2);

    // 4. 道具拾取检测
    propCollisionDetection();

    // 5. 使用全屏炸弹
    if (plane1->useBomb && plane1->superBomb > 0) {
        plane1->superBomb--;
        plane1->useBomb = false;
        // 1. 清掉所有敌机子弹
        for (auto& e : m_enemys) {
            for (auto b : e.bullets) {
                b->is_free = true;
            }
        }
        // 2. 对所有在屏敌机造成伤害
        for (auto& e : m_enemys) {
            if (!e.free) {
                e.hp -= BOMB_DAMAGE;
                if (e.hp <= 0) {
                    e.free = true;
                    Count1++;
                    // 爆炸动画
                    for (auto& bomb : m_bombs) {
                        if (bomb.free) {
                            bomb.free = false;
                            bomb.x = e.x;
                            bomb.y = e.y;
                            break;
                        }
                    }
                }
            }
        }
    }

    if (plane2->useBomb && plane2->superBomb > 0) {
        plane2->superBomb--;
        plane2->useBomb = false;
        // 1. 清掉所有敌机子弹
        for (auto& e : m_enemys) {
            for (auto b : e.bullets) {
                b->is_free = true;
            }
        }
        // 2. 对所有在屏敌机造成伤害
        for (auto& e : m_enemys) {
            if (!e.free) {
                e.hp -= BOMB_DAMAGE;
                if (e.hp <= 0) {
                    e.free = true;
                    Count2++;
                    // 爆炸动画
                    for (auto& bomb : m_bombs) {
                        if (bomb.free) {
                            bomb.free = false;
                            bomb.x = e.x;
                            bomb.y = e.y;
                            break;
                        }
                    }
                }
            }
        }
    }
    // 7. 检测胜负
    judgewhoWin();
}

void Play03::enemycollision(Plane * plane)
{
    //遍历所有非空闲的敌机
    for (int i = 0; i < ENEMY_NUM; i++) {
        if (m_enemys[i].free) {
            //空闲飞机 跳转下一次循环
            continue;
        }
        foreach (Bullet* bullet1, plane->bullets) {
            if (bullet1->is_free) {
                continue;
            }
            if (m_enemys[i].rect.intersects(bullet1->rect)) {
                m_enemys[i].free = true;
                bullet1->is_free = true;
                if (plane == plane1) {
                    Count1++;
                } else {
                    Count2++;
                }
                createProp(m_enemys[i].x, m_enemys[i].y); // 生成道具
                //播放爆炸效果
                for (int k = 0; k < BOMB_NUM; k++) {
                    if (m_bombs[k].free) {
                        m_bombs[k].free = false;
                        //更新坐标
                        m_bombs[k].x = m_enemys[i].x;
                        m_bombs[k].y = m_enemys[i].y;
                        music->playbombEffect();
                        break;
                    }
                }
                for (auto b : m_enemys[i].bullets) {
                    b->is_free = true;
                }
            }
        }
        // 敌机子弹
        for (int i = 0; i < ENEMY_NUM; i++) {
            if (m_enemys[i].free) continue;

            // 遍历该敌机的所有子弹
            foreach (EnemyBullet* bullet, m_enemys[i].bullets) {
                if (bullet->is_free) continue;

                // 检测子弹是否击中玩家飞机
                if (plane->rect.intersects(bullet->rect)) {
                    bullet->is_free = true;
                    // 触发玩家飞机爆炸（根据需求设置游戏结束或扣血）
                    if (!plane->invincible) {
                        plane->hp -= 10; // 每次击中减少10血
                    }
                    if (plane->hp <= 0) {
                        plane->hp = 0;
                        Widget::status = -1; // 血量为0时游戏结束
                    }
                }
            }
        }
    }
}

void Play03::judgewhoWin() {
    // 玩家1血量为0→玩家2胜利
    if (plane1->hp <= 0) {
        plane1->hp = 0;
        music->stopMusic(music->bgMusic);
        Widget::status = -3; // 自定义状态值：P2胜利
    }
    // 玩家2血量为0→玩家1胜利
    else if (plane2->hp <= 0) {
        plane2->hp = 0;
        music->stopMusic(music->bgMusic);
        Widget::status = -2; // 自定义状态值：P1胜利
    }
}

void Play03::createProp(int x, int y)
{
    for (int i = 0; i < PROP_NUM; ++i) {
        if (m_props[i].free) {
            m_props[i].free = false;
            m_props[i].x = x;
            m_props[i].y = y;
            break;
        }
    }
}

void Play03::propCollisionDetection()
{
    // 玩家1道具拾取检测
    for (int i = 0; i < PROP_NUM; ++i) {
        if (m_props[i].free) continue;
        if (plane1->rect.intersects(m_props[i].rect)) {
            m_props[i].free = true;
            // 根据道具类型执行效果
            switch (m_props[i].type) {
            case PROP_HP_ADD:
                plane1->hp = qMin(plane1->hp + 30, plane1->maxHp);  // 加血30（不超过上限）
                break;
            case PROP_FIRE_UP:
                plane1->firelevel = std::min(plane1->firelevel + 1, 5);
                break;
            case PROP_INVINCIBLE:
                plane1->invincible = true;
                plane1->invincibleTime = 180; // 持续3秒（60帧/秒）
                break;
            }
        }
    }

    // 玩家2道具拾取检测
    for (int i = 0; i < PROP_NUM; ++i) {
        if (m_props[i].free) continue;
        if (plane2->rect.intersects(m_props[i].rect)) {
            m_props[i].free = true;
            // 根据道具类型执行效果
            switch (m_props[i].type) {
            case PROP_HP_ADD:
                plane2->hp = qMin(plane2->hp + 30, plane2->maxHp);  // 加血30（不超过上限）
                break;
            case PROP_FIRE_UP:
                plane2->firelevel = std::min(plane2->firelevel + 1, 5);
                break;
            case PROP_INVINCIBLE:
                plane2->invincible = true;
                plane2->invincibleTime = 180; // 持续3秒（60帧/秒）
                break;
            }
        }
    }
}
