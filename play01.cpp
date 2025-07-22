#include "play01.h"
#include <QPixmap>
#include "plane.h"
#include "widget.h"
#include <QPainter>
#include <ctime>

Play01::Play01()
{
    this->plane = new Plane();
    this->boss =new Boss();
    boss2 =new Boss2();
    enter.load(":/new/res/enter.png");
    enterRect.setWidth(enter.width());
    enterRect.setHeight(enter.height());
    enterRect.moveTo(boss->x+20,boss->y+200);
    music= new Music();
    music->playbackMusic(music->bgMusic,music->bgv);


}
Play01::~Play01()
{
    delete plane;
}

void Play01::draw(QPainter &painter)
{
    plane->draw(painter);
    //count
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    int countX = WIN_W - 120; // 右边缘间距
    int countY = 70; // 上边缘间距
    QString countText = QString("得分: %1").arg(Count); // 直接使用当前类的Count
    painter.drawText(countX, countY, countText);
    // 大炸弹剩余数量
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 15));
    QString bombText = QString("剩余大炸弹:%1").arg(plane->superBomb);
    painter.drawText(WIN_W - plane->hpBarWidth - plane->hpBarMargin, plane->hpBarMargin + 65,bombText);
    for(int i = 0 ; i< ENEMY_NUM;i++)
    {
        if(m_enemys[i].free == false&&bossSpawned==false)
        {
        m_enemys[i].shoot(); // 调用发射函数
        m_enemys[i].draw(painter);
        }
        else if(bossSpawned==true){
            for(auto&e:m_enemys){
                e.free=true;
            }
        }
    }
    for(int i = 0 ; i < BOMB_NUM;i++)
    {
        if(m_bombs[i].free == false)
        {
            m_bombs[i].draw(painter);
        }
    }
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
    if (bossSpawned && !boss->alive()&&!boss2Swpawned) {
        painter.drawPixmap(boss->x+20,boss->y+200,enter);
    }
    if(boss2Swpawned&& boss2->alive()){
        boss2->draw(painter);
        frame2++;
        boss2->update(frame2,plane->x,plane->y);
    }
}

void Play01::keyPressEvent(QKeyEvent *event)
{
    int code = event->key();
    if (Widget::isPaused == false){
        // ===== 玩家1 =====
        if (code == Qt::Key_W) plane->isUp    = true;
        if (code == Qt::Key_S) plane->isDown  = true;
        if (code == Qt::Key_A) plane->isLeft  = true;
        if (code == Qt::Key_D) plane->isRight = true;
        if (code == Qt::Key_E) {
            plane->isFire = true;
            music->playshootEffect();
        }
        if(code == Qt::Key_Q) {
            if(plane->superBomb>0) plane->useBomb=true;
        }
    }
}
void Play01::keyReleaseEvent(QKeyEvent *event)
{
    int code = event->key();
    if (Widget::isPaused == false){
        // ===== 玩家1 =====
        if (code == Qt::Key_W) plane->isUp    = false;
        if (code == Qt::Key_S) plane->isDown  = false;
        if (code == Qt::Key_A) plane->isLeft  = false;
        if (code == Qt::Key_D) plane->isRight = false;
        if(code == Qt::Key_Q) {
            if(plane->superBomb>0) plane->useBomb=false;
        }
    }
}

void Play01::enemyToScene()
{
    if (Widget::isPaused||bossSpawned) return;
    m_recorder++;
    if(m_recorder < enemyintereval)
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
    //boss战
    if (Count >= 36 && !bossSpawned) {
        music->stopMusic(music->bgMusic);
        music->playbackMusic(music->bossMusic,music->bossv);
        bossSpawned = true;
        boss->hp=BOSS_MAX_HP;
        // 可重置敌机，进入BOSS战
        for (int i = 0; i < ENEMY_NUM; ++i) {
            m_enemys[i].free = true;
        }
    }
}
void Play01::collisionDetection()
{
    if (Widget::isPaused) return;
    //遍历所有非空闲的敌机
    for(int i = 0 ;i < ENEMY_NUM;i++)
    {
        if(m_enemys[i].free)
        {
            //空闲飞机 跳转下一次循环
            continue;
        }
        if(m_enemys[i].rect.intersects(plane->rect))//敌机与飞机相撞
        {
            m_enemys[i].free = true;
            plane->hp=0;
            music->stopMusic(music->bossMusic);
            music->stopMusic(music->bgMusic);
            //播放爆炸效果
            for(int k = 0 ; k < BOMB_NUM;k++)
            {
                if(m_bombs[k].free)
                {
                    m_bombs[k].free = false;
                    //更新坐标
                    m_bombs[k].x = m_enemys[i].x;
                    m_bombs[k].y = m_enemys[i].y;
                    break;                  
                }
            }
            Widget::status = -1; // 血量为0时游戏结束
        }
        foreach (Bullet* bullet1, plane->bullets) {
            if(bullet1->is_free){
                continue;
            }
            if(m_enemys[i].rect.intersects(bullet1->rect))//敌机与子弹相撞
            {
                bullet1->is_free = true;
                m_enemys[i].free =true;
                createProp(m_enemys[i].x, m_enemys[i].y);//生成道具
                Count++;
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
                for(auto b : m_enemys[i].bullets){
                    b->is_free=true;
                }
            }

        }
        //dijizidan
        for(int i = 0; i < ENEMY_NUM; i++) {
            if(m_enemys[i].free) continue;

            // 遍历该敌机的所有子弹
            foreach (EnemyBullet* bullet, m_enemys[i].bullets) {
                if (bullet->is_free) continue;

                // 检测子弹是否击中玩家飞机
                if (plane->rect.intersects(bullet->rect)) {
                    bullet->is_free = true;
                    // 触发玩家飞机爆炸（根据需求设置游戏结束或扣血）
                    if(!plane->invincible){
                    plane->hp -= 10; // 每次击中减少20血
                    }
                    if (plane->hp <= 0) {
                        plane->hp = 0;
                        Widget::status = -1; // 血量为0时游戏结束
                    }
                }
            }
        }
        //道具拾取检测
        for (int i = 0; i < PROP_NUM; ++i) {
            if (m_props[i].free) continue;
            if (plane->rect.intersects(m_props[i].rect)) {
                m_props[i].free = true;
                // 根据道具类型执行效果
                switch (m_props[i].type) {
                case PROP_HP_ADD:
                    plane->hp = qMin(plane->hp + 30, plane->maxHp);  // 加血30（不超过上限）
                    break;
                case PROP_FIRE_UP:
                    plane->firelevel=std::min(plane->firelevel+1,5);
                    break;
                    // 在Plane类添加无敌状态标记
                case PROP_INVINCIBLE:
                    plane->invincible = true;
                    plane->invincibleTime = 180; // 持续3秒（60帧/秒）
                    break;
                    break;
                }
            }
        }
        //使用全屏炸弹
        if(plane->useBomb && plane->superBomb>0){
            plane->superBomb--;
        }
        // 处理玩家使用全屏炸弹
        if(plane->useBomb){
            plane->useBomb=false;
            // 1. 清掉所有敌机子弹
            for(auto& e : m_enemys){
                for(auto b : e.bullets){
                    b->is_free=true;
                }
            }

            // 2. 对所有在屏敌机造成伤害
            for(auto& e : m_enemys){
                if(!e.free){
                    e.hp -= BOMB_DAMAGE;
                    if(e.hp <=0){
                        e.free=true;
                        Count++;
                        // 爆炸动画
                        for(auto& bomb : m_bombs){
                            if(bomb.free){
                                bomb.free=false;
                                bomb.x=e.x;
                                bomb.y=e.y;
                                break;
                            }
                        }
                    }
                }
            }
            if (bossSpawned && boss->alive())
            {
                boss->takeDamage(10+(10*plane->firelevel));
            }
        }      
    }
    // BOSS与玩家子弹碰撞
    if (bossSpawned && boss->alive()) {
        // 玩家子弹击中Boss
        foreach (Bullet* bullet, plane->bullets) {
            if (bullet->is_free) continue;
            // 检测子弹与Boss碰撞
            QRect bossRect(boss->x, boss->y, boss->img.width(), boss->img.height());
            if (bossRect.intersects(bullet->rect)) {
                bullet->is_free = true;
                boss->takeDamage(10+(10*plane->firelevel));  // 每发子弹造成20伤害
                music->playbombEffect();
                // Boss死亡时触发爆炸和得分
                if (!boss->alive()) {
                    music->stopMusic(music->bossMusic);
                    music->playbackMusic(music->bgMusic,music->bgv);
                    Count += 100;  // 击杀Boss加分
                    for (int k = 0; k < BOMB_NUM; ++k) {
                        if (m_bombs[k].free) {
                            m_bombs[k].free = false;
                            m_bombs[k].x = boss->x;
                            m_bombs[k].y = boss->y;
                            break;
                        }
                    }
                }
            }
        }
        // Boss弹幕击中玩家
        foreach (BossBullet* b, boss->bullets) {
            if (plane->rect.intersects(b->rect) && !plane->invincible) {
                plane->hp -= 1;  // 被Boss弹幕击中扣15血
                if (plane->hp <= 0) {
                    plane->hp = 0;
                    Widget::status = -1;  // 游戏结束
                    music->stopMusic(music->bossMusic);
                }
            }
        }
    }
    //进入精英关卡
    if(plane->rect.intersects(enterRect)&&bossSpawned&&!boss->alive()&&!boss2Swpawned){
        plane->hp=100;
        boss2Swpawned = true;
    }
    // BOSS2与玩家子弹碰撞
    if (boss2Swpawned && boss2->alive()) {
        // 玩家子弹击中Boss
        foreach (Bullet* bullet, plane->bullets) {
            if (bullet->is_free) continue;
            // 检测子弹与Boss碰撞
            QRect bossRect(boss2->m_x, boss2->m_y, boss2->img.width(), boss2->img.height());
            if (bossRect.intersects(bullet->rect)) {
                bullet->is_free = true;
                boss2->takeDamage(80+(30*plane->firelevel));  // 每发子弹造成20伤害
                music->playbombEffect();
                // Boss死亡时触发爆炸和得分
                if (!boss2->alive()) {
                    music->stopMusic(music->bossMusic);
                    music->playbackMusic(music->bgMusic,music->bgv);
                    Count += 200;  // 击杀Boss加分
                    for (int k = 0; k < BOMB_NUM; ++k) {
                        if (m_bombs[k].free) {
                            m_bombs[k].free = false;
                            m_bombs[k].x = boss2->m_x;
                            m_bombs[k].y = boss2->m_y;
                            break;
                        }
                    }
                    for (auto bullet : boss2->bullets) {
                        delete bullet;
                    }
                    boss2->bullets.clear();
                }
            }
        }
        // Boss2弹幕击中玩家
        for (auto it = boss2->bullets.begin(); it != boss2->bullets.end();) {
            BossBullet2* b = *it;
            bool collided = false;
            // 1. 先检测与玩家子弹的碰撞
            for (auto bullet : plane->bullets) {
                if (bullet->is_free) continue;  // 跳过空闲子弹
                if (bullet->rect.intersects(b->rect)) {
                    bullet->is_free = true;     // 标记玩家子弹为空闲
                    delete b;                   // 删除 Boss2 弹幕
                    it = boss2->bullets.erase(it);  // 更新迭代器
                    collided = true;
                    break;  // 已处理碰撞，跳出玩家子弹循环
                }
            }
            if (collided) continue;  // 已处理，继续下一个 Boss2 弹幕
            // 2. 再检测与玩家飞机的碰撞
            if (plane->rect.intersects(b->rect) && !plane->invincible) {
                plane->hp -= 5;
                delete b;
                it = boss2->bullets.erase(it);
                if (plane->hp <= 0) {
                    plane->hp = 0;
                    Widget::status = -1;
                    music->stopMusic(music->bossMusic);
                }
                collided = true;
            }
            // 3. 未碰撞则移动迭代器
            if (!collided) {
                ++it;
            }
        }
    }
}
// 击败敌机后随机生成道具
void Play01::createProp(int x, int y) {
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
void Play01::gamelevel(){
    if(Count>=10&&counter==0){
        enemyintereval-=10;
        counter=1;
        for(auto& e:m_enemys){
            e.fireDelay-=20;
        }
    }
    if(Count>=20&&counter==1){
        enemyintereval-=10;
        counter=2;
        for(auto& e:m_enemys){
            e.fireDelay-=20;
        }
    }
    if(Count>=30&&counter==2){
        enemyintereval-=10;
        counter=3;
    }
}


