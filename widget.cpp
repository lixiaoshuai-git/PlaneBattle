#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QICon>
#include <ctime>
#include <cstdlib>
#include <QMessageBox>
#include "before.h"
#include "play01.h"
#include "over.h"
#include "map.h"
int Widget::status =0;
bool Widget::isPaused = false;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("飞机大战");
    setWindowIcon(QIcon(":/new/res/app.ico"));
    this->resize(600,800);
    //    把最大宽和 高设置成相同
    this->setMinimumWidth(WIN_W);
    this->setMaximumWidth(WIN_W);
    this->setMinimumHeight(WIN_H);
    this->setMaximumHeight(WIN_H);
    btn = new QPushButton("操作说明",this);
    btn->move(180,200);
    btn->resize(150,50);
    connect(btn, &QPushButton::clicked, this, [this]() {
        music->playselectEffect();
        QMessageBox::information(
            this,                                   // 父窗口
            "游戏玩法说明",                         // 窗口标题
            "\n\n"                // 内容（支持 \n 换行）
            "操作方式：\n"
            "  - 方向键WASD 或 8456 移动\n"
            "  - E 或 0 发射子弹\n"
            "  - Q 使用炸弹\n\n"
            "游戏目标：\n"
            "  - 击败尽可能多的敌人\n"
            "  - 击败最终Boss\n\n"
            "游戏模式：\n"
            "  - 单人模式：一名玩家挑战不断出现的敌机，以获取更高分数为目标。\n"
            "  - 双人合作模式 (Co-op): 两名玩家共同迎战敌机，共享屏幕，但生命值和得分各自独立计算。考验团队配合。\n"
            "  - 双人对战模式 (VS):玩家之间可以互相攻击，同时也要应对中立的敌机。目标是击败对方玩家。\n"
            );
    });

    // 初始化暂停菜单按钮
    continueBtn = new QPushButton("继续游戏", this);
    menuBtn = new QPushButton("返回菜单", this);

    continueBtn->move(180, 450);
    menuBtn->move(180, 550);
    continueBtn->resize(150, 50);
    menuBtn->resize(150, 50);

    continueBtn->setVisible(false);
    menuBtn->setVisible(false);

    continueBtn->setFocusPolicy(Qt::NoFocus);
    menuBtn->setFocusPolicy(Qt::NoFocus);

    // 连接按钮信号
    connect(continueBtn, &QPushButton::clicked, this, [this]() {
        music->playselectEffect();
        isPaused = false;
        hidePauseMenu();
    });

    connect(menuBtn, &QPushButton::clicked, this, [this]() {
        music->playselectEffect();
        isPaused = false;
        hidePauseMenu();
        if(play01&&play01->music->bgMusic->isPlaying()==true)play01->music->stopMusic(play01->music->bgMusic);
        else if(play01&&play01->music->bossMusic->isPlaying()==true)play01->music->stopMusic(play01->music->bossMusic);
        else if(play02&&play02->music->bgMusic->isPlaying()==true)play02->music->stopMusic(play02->music->bgMusic);
        else if(play03&&play03->music->bgMusic->isPlaying()==true)play03->music->stopMusic(play03->music->bgMusic);
        Widget::status = 0;  // 返回主菜单
        music->playbackMusic(music->menuMusic,music->menuv);
    });

    //单人
    btn1 = new QPushButton("单人模式", this);
    btn1->move(180, 300);
    btn1->resize(150, 50);
    connect(btn1, &QPushButton::clicked, this, [this](){
        music->playselectEffect();
        this->play01 = new Play01();
        music->stopMusic(music->menuMusic);
        map->level=1;
        map->reloadMap();
        Widget::status = 1;
        play01->plane->hp=100;
        play01->Count=0;
        update();
    });
    //双人
    btn2 = new QPushButton("双人合作", this);
    btn2->move(180, 400); // 位置在单人按钮下方
    btn2->resize(150, 50);
    btn2->setVisible(false); // 初始隐藏
    // 绑定点击事件，切换到双人模式（status=2）
    connect(btn2, &QPushButton::clicked, this, [this](){
        music->playselectEffect();
        Widget::status = 2;
        music->stopMusic(music->menuMusic);
        this->play02 = new Play02();
        play02->plane1->hp=100;
        play02->plane2->hp=100;
        update();
    });
    //对战
    btn3 = new QPushButton("双人对战", this);
    btn3->move(180, 500); // 位置在双人按钮下方
    btn3->resize(150, 50);
    btn3->setVisible(false); // 初始隐藏
    // 绑定点击事件，切换到双人模式（status=2）
    connect(btn3, &QPushButton::clicked, this, [this](){
        music->playselectEffect();
        Widget::status = 3;
        music->stopMusic(music->menuMusic);
        this->play03 = new Play03();
        play03->plane1->hp=100;
        play03->plane2->hp=100;
        update();
    });
    //历史记录
    btn4 = new QPushButton("历史记录",this);
    btn4->move(180,600);
    btn4->resize(150,50);
    connect(btn4,&QPushButton::clicked,this,[=](){
        music->playselectEffect();
        int highScore = scoreManager::getHighScore();
        QMessageBox::information(this,
                                 "最高记录",
                                 QString("游戏最高分: %1 分").arg(highScore));
    });
    btn1->setFocusPolicy(Qt::NoFocus);
    btn2->setFocusPolicy(Qt::NoFocus);
    btn3->setFocusPolicy(Qt::NoFocus);
    btn->setFocusPolicy(Qt::NoFocus);
    btn4->setFocusPolicy(Qt::NoFocus);
    this->map = new Map();
    this->over = new Over(this);
    // 接收OVER类中返回主菜单信号，点击按钮后切换到主菜单状态（status=0）
    connect(over, &Over::backToMainMenu, this, [this](){
        music->playselectEffect();
        Widget::status = 0;
        this->before = new Before();
        music->playbackMusic(music->menuMusic,music->menuv);
    });
    this->startTimer(20);//1秒后触发 某个函数
    music = new Music();
    music->playbackMusic(music->menuMusic,music->menuv);

}
void Widget::timerEvent(QTimerEvent *event)
{
    this->update();
    if(status==1){
        play01->enemyToScene();
        play01->gamelevel();
        // 在Widget的timerEvent中更新所有爆炸
        for (int i = 0; i < BOMB_NUM; ++i) {
            if (!play01->m_bombs[i].free) {
                play01->m_bombs[i].updateInfo();
            }
        }
        play01->collisionDetection();

    }
    if(status==2){
        play02->enemyToScene();
        play02->collisionDetection();
        for (int i = 0; i < BOMB_NUM; ++i) {
            if (!play02->m_bombs[i].free) {
                play02->m_bombs[i].updateInfo();
            }
        }
    }
    if (status == 3) {
        play03->enemyToScene();
        play03->collisionDetection();
        for (int i = 0; i < BOMB_NUM; ++i) {
            if (!play03->m_bombs[i].free) {
                play03->m_bombs[i].updateInfo();
            }
        }
    }
}

void Widget::showPauseMenu()
{
    continueBtn->setVisible(true);
    menuBtn->setVisible(true);
    isPaused = true;
}

void Widget::hidePauseMenu()
{
    continueBtn->setVisible(false);
    menuBtn->setVisible(false);
    isPaused = false;
}

int Widget::getRandomInt()
{
    // 生成 0-7 之间的随机整数并返回
    int ranInt = std::rand() % 7+1;
    return ranInt;
}
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //

    if(isPaused && (status == 1 || status == 2 || status == 3)) {
        // 先绘制游戏画面
        if(status == 1) {
            map->draw(painter);
            play01->draw(painter);
        }
        else if(status == 2) {
            map->draw(painter);
            play02->draw(painter);
        }
        else if(status == 3) {
            map->draw(painter);
            play03->draw(painter);
        }

        // 再绘制半透明覆盖层
        painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0, 150));

        // 绘制"游戏暂停"文字
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 30, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "游戏暂停");

        continueBtn->setVisible(true);
        menuBtn->setVisible(true);
        return;
    }


    if(Widget::status == 0){
        before->draw(painter);
        btn->setVisible(true);
        btn1->setVisible(true);
        btn2->setVisible(true);
        btn3->setVisible(true);
        btn4->setVisible(true);
        over->backBtn->setVisible(false);        // 显示按钮
    }
    else if(Widget::status == 1){
        btn->setVisible(false);
        btn2->setVisible(false);        // 隐藏按钮
        btn1->setVisible(false);
        btn3->setVisible(false);
        btn4->setVisible(false);
        over->backBtn->setVisible(false);
        //根据得分切换地图
        if(play01->Count>=10&&map->level==1) {
            map->level=2;
            map->reloadMap();
        }
        if(play01->Count>=20&&map->level==2) {
            map->level=3;
            map->reloadMap();
        }
        if(play01->Count>=30&&map->level==3) {
            map->level=5;
            map->reloadMap();
        }
        map->draw(painter);
        map->map_pos();
        play01->draw(painter);
    }
    else if(Widget::status == 2){
        btn->setVisible(false);
        btn2->setVisible(false);        // 隐藏按钮
        btn1->setVisible(false);
        btn3->setVisible(false);
        btn4->setVisible(false);
        if(map->level!=4){
        map->level=4;
        map->reloadMap();
        }
        map->draw(painter);
        map->map_pos();
        play02->draw(painter);
    }
    else if (Widget::status == -2 || Widget::status == -3) {
        // 显示胜利结果（传递status给over->draw）
        over->draw(painter, Widget::status);
    }
    else if(Widget::status == -1){
        btn->setVisible(false);
        btn2->setVisible(false); // 隐藏按钮
        btn1->setVisible(false);
        btn3->setVisible(false);
        btn4->setVisible(false);
        over->draw(painter,play01->Count);
    }
    else if (status == 3) {
        btn->setVisible(false);
        btn2->setVisible(false); // 隐藏按钮
        btn1->setVisible(false);
        btn3->setVisible(false);
        btn4->setVisible(false);
        over->backBtn->setVisible(false);
        map->draw(painter);
        map->map_pos();
        play03->draw(painter);
    }
    else if(Widget::status == -4){
        btn->setVisible(false);
        btn2->setVisible(false); // 隐藏按钮
        btn1->setVisible(false);
        btn3->setVisible(false);
        btn4->setVisible(false);
        over->draw(painter,-4);
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(Widget::status==1){
        play01->keyPressEvent(event);
    }
    else if (status == 2) {
        play02->keyPressEvent(event);
    }
    else if(Widget::status==-1){
        over->keyPressEvent(event);
    }
    else if (status == 3) {
        play03->keyPressEvent(event);
    }
    if(event->key() == Qt::Key_Escape) {
        if(Widget::status == 1 || Widget::status == 2 || Widget::status == 3) {
            isPaused = true;
            update();
        }
    }
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{

    if(Widget::status==1){
        play01->keyReleaseEvent(event);
    }
    else if (status == 2) {
        play02->keyReleaseEvent(event);
    }
    else if(Widget::status==-1){
        over->keyReleaseEvent(event);
    }
     else if (status == 3) {
        play03->keyReleaseEvent(event);
    }
    }
Widget::~Widget()
{
    delete ui;
    delete before;
    delete play01;
    delete play02;
    delete play03;
    delete over;
    delete map;
    delete continueBtn;
    delete menuBtn;
}


