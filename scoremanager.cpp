#include "scoreManager.h"
const QString scoreManager::FILE_PATH = "highscore.dat";
int scoreManager::getHighScore() {
    QFile file(FILE_PATH);
    if (!file.open(QIODevice::ReadOnly)) {
        return 0; // 文件不存在时返回0
    }
    QTextStream in(&file);
    int score = in.readLine().toInt();
    file.close();
    return score;
}
void scoreManager::saveHighScore(int score) {
    QFile file(FILE_PATH);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return; // 写入失败时静默处理
    }
    QTextStream out(&file);
    out << score;
    file.close();
}
