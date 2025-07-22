#ifndef OVER_H
#define OVER_H
#include <QPainter>
#include <QKeyEvent>
#include <QPushButton>
#include <QObject>
#include "scoremanager.h"
class Over : public QObject {
    Q_OBJECT
public:
    Over(QWidget *parent = nullptr); // 接收父窗口指针，用于按钮显示
    void draw(QPainter &painter, int result);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    // 按钮,用于在主窗口中设置位置和可见性
    QPushButton *backBtn;
signals:
    void backToMainMenu(); // 返回主菜单的信号

private:
    QWidget *parentWidget; // 父窗口（用于显示按钮）
};
#endif // OVER_H
