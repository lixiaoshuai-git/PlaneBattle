#include "Play02.h"

#include <QPixmap>

#include "plane.h"

#include "widget.h"

#include <QPainter>

#include <ctime>

int count1 = 0;

int count2 = 0;

Play02::Play02()

{
    // 初始化随机种子（关键！确保每次运行随机序列不同）
    srand((unsigned int)time(nullptr));
    this->plane1 = new Plane();

    this->plane2 = new Plane();


    music = new Music();
    music->playbackMusic(music->bgMusic, music->bgv);

    plane1->image.load(":/new/res/hero.png");

    plane2->image.load(":/new/res/hero2.png");
    // 初始位置区分
    plane1->x = 150;
    plane1->y = 600;
    plane1->hp = plane1->maxHp = 100;
    plane2->x = 300;

    plane2->y = 600;

    plane2->hp = plane2->maxHp = 100;





}

Play02::~Play02()

{

    delete plane1;

    delete plane2;

}



void Play02::draw(QPainter &painter)

{


    // 大炸弹剩余数量
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 15));
    QString bombText1 = QString("剩余大炸弹:%1").arg(plane1->superBomb);
    painter.drawText(plane1->hpBarMargin, plane1->hpBarMargin + 65,bombText1);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 15));
    QString bombText2 = QString("剩余大炸弹:%1").arg(plane2->superBomb);
    painter.drawText(WIN_W - plane2->hpBarWidth - plane2->hpBarMargin, plane2->hpBarMargin + 65,bombText2);


    // 只在血量 > 0 时绘制飞机

    if (plane1-> isAlive) {

        plane1->drawPlayer2(painter);
    }

    if (plane2->isAlive) {

        plane2->draw(painter);

    }

    Count = count1 + count2;

    int countX1 = hpBarMargin; // 左边缘间距

    int countY = 70; // 上边缘间距

    QString countText1 = QString("玩家1得分: %1").arg(count1); // 直接使用当前类的Count

    painter.drawText(countX1, countY, countText1);

    int countX2 = WIN_W - hpBarWidth - hpBarMargin; // 右边缘间距

    QString countText2 = QString("玩家2得分: %1").arg(count2); // 直接使用当前类的Count

    painter.drawText(countX2, countY, countText2);

    for(int i = 0 ; i< ENEMY_NUM;i++)

    {

        if(m_enemys[i].free == false)

        {

            m_enemys[i].shoot(); // 调用发射函数

            m_enemys[i].draw(painter);

        }

    }

    for(int i = 0 ; i < BOMB_NUM;i++)

    {

        if(m_bombs[i].free == false)

        {
            m_bombs[i].draw(painter);
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


}



void Play02::keyPressEvent(QKeyEvent *event)

{

    int code = event->key();

    if (plane1->isAlive){

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


    }

    // ===== 玩家2 =====

    if (plane2->isAlive){

        if (code == Qt::Key_Up)  plane2->isUp    = true;

        if (code == Qt::Key_Down)  plane2->isDown  = true;

        if (code == Qt::Key_Left)  plane2->isLeft  = true;

        if (code == Qt::Key_Right)  plane2->isRight = true;

        if (code == Qt::Key_Plus){
            plane2->isFire = true;
            music->playshootEffect();
        }
        if (code == Qt::Key_Enter) {
            if (plane2->superBomb > 0) plane2->useBomb = true;
        }
    }

}



void Play02::keyReleaseEvent(QKeyEvent *event)

{

    int code = event->key();

    // ===== 玩家1 =====

    if (plane1->isAlive){

        if (code == Qt::Key_W) plane1->isUp    = false;

        if (code == Qt::Key_S) plane1->isDown  = false;

        if (code == Qt::Key_A) plane1->isLeft  = false;

        if (code == Qt::Key_D) plane1->isRight = false;

    }



    // ===== 玩家2 =====

    if (plane2->isAlive){

        if (code == Qt::Key_Up)  plane2->isUp    = false;

        if (code == Qt::Key_Down)  plane2->isDown  = false;

        if (code == Qt::Key_Left)  plane2->isLeft  = false;

        if (code == Qt::Key_Right)  plane2->isRight = false;

        if (code == Qt::Key_Plus) plane2->isFire = false;

    }

}

void Play02::enemyToScene()

{

    m_recorder++;

    if(m_recorder < ENEMY_INTERVAL)

    {
        return;
    }



    m_recorder = 0;



    for(int i = 0 ; i< ENEMY_NUM;i++)

    {

        if(m_enemys[i].free)

        {

            //敌机空闲状态改为false

            m_enemys[i].free = false;

            //设置坐标

            m_enemys[i].x = rand() % (WIN_W - m_enemys[i].rect.width());

            m_enemys[i].y = -m_enemys[i].rect.height();

            break;

        }

    }

}

void Play02::collisionDetection()
{
    //遍历所有非空闲的敌机
    for(int i = 0 ;i < ENEMY_NUM;i++)
    {
        if(m_enemys[i].free)
        {
            //空闲飞机 跳转下一次循环
            continue;
        }
        if (plane1->isAlive){
            foreach (Bullet* bullet1, plane1->bullets) {
                if(bullet1->is_free){
                    continue;
                }
                if(m_enemys[i].rect.intersects(bullet1->rect))
                {
                    m_enemys[i].free = true;
                    bullet1->is_free = true;
                    count1++;
                    createProp(m_enemys[i].x, m_enemys[i].y); // 生成道具
                    //播放爆炸效果
                    for(int k = 0 ; k < BOMB_NUM;k++)
                    {
                        if(m_bombs[k].free)
                        {
                            m_bombs[k].free = false;
                            //更新坐标
                            m_bombs[k].x = m_enemys[i].x;
                            m_bombs[k].y = m_enemys[i].y;
                            music->playbombEffect();
                            break;
                        }
                    }
                }
            }
        }
        for(int i = 0; i < ENEMY_NUM; i++) {
            if(m_enemys[i].free) continue;
            // 遍历该敌机的所有子弹
            if (plane1->isAlive){
                foreach (EnemyBullet* bullet, m_enemys[i].bullets) {
                    if (bullet->is_free) continue;
                    // 检测子弹是否击中玩家飞机
                    if (plane1->rect.intersects(bullet->rect)) {
                        bullet->is_free = true;
                        // 触发玩家飞机爆炸（根据需求设置游戏结束或扣血）
                        if (!plane1->invincible) {
                            plane1->hp -= 10; // 每次击中减少10血
                        }
                        if (plane1->hp <= 0) {
                            plane1->hp = 0;
                            plane1->isAlive = false; // 标记为死亡
                        }
                        if (plane1->hp <= 0 && plane2->hp <= 0){
                            Widget::status = -4; // 血量为0时游戏结束
                        }
                    }
                }
            }
        }
        if(m_enemys[i].free)
        {
            //空闲飞机 跳转下一次循环
            continue;
        }
        if (plane2->isAlive){
            foreach (Bullet* bullet1, plane2->bullets) {
                if(bullet1->is_free){
                    continue;
                }
                if(m_enemys[i].rect.intersects(bullet1->rect))
                {
                    m_enemys[i].free = true;
                    bullet1->is_free = true;
                    count2++;
                    createProp(m_enemys[i].x, m_enemys[i].y); // 生成道具
                    //播放爆炸效果
                    for(int k = 0 ; k < BOMB_NUM;k++)
                    {
                        if(m_bombs[k].free)
                        {
                            m_bombs[k].free = false;
                            //更新坐标
                            m_bombs[k].x = m_enemys[i].x;
                            m_bombs[k].y = m_enemys[i].y;
                            music->playbombEffect();
                            break;
                        }
                    }
                }
            }
        }

        for(int i = 0; i < ENEMY_NUM; i++) {

            if(m_enemys[i].free) continue;
            // 遍历该敌机的所有子弹
            if (plane2->isAlive){
                foreach (EnemyBullet* bullet, m_enemys[i].bullets) {
                    if (bullet->is_free) continue;
                    // 检测子弹是否击中玩家飞机
                    if (plane2->rect.intersects(bullet->rect)) {
                        bullet->is_free = true;
                        // 触发玩家飞机爆炸（根据需求设置游戏结束或扣血）
                        if (!plane2->invincible) {
                            plane2->hp -= 10; // 每次击中减少10血
                        }
                        if (plane2->hp <= 0) {
                            plane2->hp = 0;
                            plane2->isAlive = false; // 标记为死亡
                        }
                        if (plane1->hp <= 0 && plane2->hp <= 0){
                            Widget::status = -4; // 血量为0时游戏结束
                        }
                    }
                }
            }
        }
    }
    //  道具拾取检测
    propCollisionDetection();
    //  使用全屏炸弹
    if (plane1->useBomb && plane1->superBomb > 0) {
        plane1->superBomb--;
        plane1->useBomb = false;
        //  清掉所有敌机子弹
        for (auto& e : m_enemys) {
            for (auto b : e.bullets) {
                b->is_free = true;
            }
        }
        //  对所有在屏敌机造成伤害
        for (auto& e : m_enemys) {
            if (!e.free) {
                e.hp -= BOMB_DAMAGE;
                if (e.hp <= 0) {
                    e.free = true;
                    count1++;
                    // 爆炸动画
                    for (auto& bomb : m_bombs) {
                        if (bomb.free) {
                            bomb.free = false;
                            bomb.x = e.x;
                            bomb.y = e.y;
                            music->playbombEffect();
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
        //  清掉所有敌机子弹
        for (auto& e : m_enemys) {
            for (auto b : e.bullets) {
                b->is_free = true;
            }
        }
        //  对所有在屏敌机造成伤害
        for (auto& e : m_enemys) {
            if (!e.free) {
                e.hp -= BOMB_DAMAGE;
                if (e.hp <= 0) {
                    e.free = true;
                    count2++;
                    // 爆炸动画
                    for (auto& bomb : m_bombs) {
                        if (bomb.free) {
                            bomb.free = false;
                            bomb.x = e.x;
                            bomb.y = e.y;
                            music->playbombEffect();
                            break;
                        }
                    }
                }
            }
        }
    }
}

void Play02::createProp(int x,int y)
{
    // 随机判断是否掉落道具（40%概率）
    if (rand() % 100 >= PROP_DROP_RATE) {
        return;
    }
    // 查找空闲道具位置
    for (int i = 0; i < PROP_NUM; ++i) {
        if (m_props[i].free) {
            m_props[i].free = false;
            m_props[i].x = x+30;  // 敌机位置为道具初始位置
            m_props[i].y = y+30;
            m_props[i].speed = 2;  // 掉落速度
            break;
        }
    }
}

void Play02::propCollisionDetection()
{

    // 先更新所有碰撞矩形
    plane1->rect.moveTo(plane1->x, plane1->y);
    plane2->rect.moveTo(plane2->x, plane2->y);

    // 玩家1道具拾取检测
    for (int i = 0; i < PROP_NUM; ++i) {
        if (m_props[i].free) continue;
        m_props[i].rect.moveTo(m_props[i].x, m_props[i].y); // 更新道具位置
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
        m_props[i].rect.moveTo(m_props[i].x, m_props[i].y); // 更新道具位置
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
