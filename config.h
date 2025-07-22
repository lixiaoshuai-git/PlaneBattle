#ifndef CONFIG_H
#define CONFIG_H

#define PLANE_MOVE_SPEED 5
#define WIN_W 510
#define WIN_H 750
#define MAP_SPEED 2
#define BULLET_SPEED 10
#define BULLET_NUM 30
#define BULLET_VAL 10
#define ENEMY_NUM 20
#define ENEMY_INTERVAL  90  //敌机出场时间间隔
#define BOMB_PATH  ":/res/bomb-%1.png"   //炸弹资源图片
#define BOMB_NUM  20     //爆炸数量
#define BOMB_MAX  7      //爆炸图片最大索引
#define BOMB_INTERVAL 2   //爆炸切图时间间隔
#define PROP_NUM 10         // 最大道具数量
#define PROP_DROP_RATE 40   // 掉落概率（百分比）

#define PROP_HP_ADD 0         // 加血道具
#define PROP_FIRE_UP 1  // 射速提升道具
#define PROP_INVINCIBLE 2     // 无敌道具
#define MAX_BOMB      100         // 每玩家最大携带炸弹数
#define BOMB_DAMAGE   100        // 全屏炸弹对敌机的伤害

#define BOSS_NUM        1            // 每关 1 个 BOSS
#define BOSS_MAX_HP     900          // BOSS 总血量
#define BOSS_MAX_HP2     1400          // BOSS 总血量
#define BOSS_PHASE_HP2   700          // 每阶段血量
#define BOSS_PHASE_HP   300          // 每阶段血量
#define BOSS_BULLET_NUM 200          // BOSS 弹幕池
#define BOSS_BULLET_SPD 6
#endif // CONFIG_H
