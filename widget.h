#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"before.h"
#include"play01.h"
#include"over.h"
#include "map.h"
#include "play03.h"
#include "play02.h"
#include <QPushButton>
#include "scoremanager.h"
#include "music.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    static int status;
    QPushButton *btn;
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;
    explicit Widget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *painter);
    int getRandomInt();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);// alt +回车
    void timerEvent(QTimerEvent *event);
    Before *before = nullptr;
    Play01 *play01 = nullptr;
    Play02 *play02 = nullptr;
    Play03 *play03 = nullptr;
    Over *over = nullptr;
    Map *map = nullptr;
    Music *music =nullptr;

    static bool isPaused;  // 添加暂停状态标志
    QPushButton *continueBtn = nullptr;  // 继续游戏按钮
    QPushButton *menuBtn = nullptr;     // 返回菜单按钮
    void showPauseMenu();    // 显示暂停菜单
    void hidePauseMenu();    // 隐藏暂停菜单

    ~Widget();



private:
    Ui::Widget *ui;
};


#endif // WIDGET_H
