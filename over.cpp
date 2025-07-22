#include "over.h"
#include <QFont>
#include <QPalette>

Over::Over(QWidget *parent) : QObject(parent), parentWidget(parent) {
    // 初始化返回按钮
    backBtn = new QPushButton("返回主菜单", parentWidget);
    backBtn->setVisible(false); // 初始隐藏（仅在游戏结束时显示）
    backBtn->resize(150, 50); // 设置按钮大小
    backBtn->setFont(QFont("宋体", 12));

    // 设置按钮样式（白色文字+深色背景，便于在游戏背景上显示）
    QPalette pal = backBtn->palette();
    pal.setColor(QPalette::ButtonText, Qt::white);
    pal.setColor(QPalette::Button, Qt::darkGray);
    backBtn->setPalette(pal);
    backBtn->setStyleSheet("border: 2px solid white;");
    // 绑定按钮点击事件：发射返回主菜单信号
    connect(backBtn, &QPushButton::clicked, this, &Over::backToMainMenu);
}
void Over::draw(QPainter &painter, int result) {
    // 绘制胜负结果文字
    painter.setPen(Qt::white);
    painter.setFont(QFont("宋体", 30, QFont::Bold));
    if (result == -2) {
        painter.drawText(150, 100, "玩家1胜利！");
    } else if (result == -4) {
        painter.drawText(150, 100, "游戏结束");
    } else if (result == -3) {
        painter.drawText(150, 100, "玩家2胜利！");
    } else {
        // 获取历史最高分
        int highScore = scoreManager::getHighScore();
        bool isNewRecord = (result > highScore);

        // 更新最高分
        if (isNewRecord) {
            int newHighScore = result;
            scoreManager::saveHighScore(newHighScore);
        }
        QString textover = QString("游戏结束，得分为%1").arg(result);
        painter.drawText(80, 100, textover);
    }
    // 显示返回按钮（居中显示在屏幕下方）
    backBtn->setVisible(true);
    int btnX = (parentWidget->width() - backBtn->width()) / 2; // 水平居中
    int btnY = parentWidget->height() - 100; // 距离底部100像素
    backBtn->move(btnX, btnY);
}

void Over::keyPressEvent(QKeyEvent *event) {}
void Over::keyReleaseEvent(QKeyEvent *event) {}
