#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H
#pragma once
#include <QString>
#include <QFile>
#include <QTextStream>

class scoreManager {
public:
    static int getHighScore();
    static void saveHighScore(int score);
private:
    static const QString FILE_PATH; // 保存分数的文件路径
};

#endif // SCOREMANAGER_H
