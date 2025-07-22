#ifndef BEFORE_H
#define BEFORE_H
#include <qpainter>
#include <QKeyEvent>
class Widget;
class Before {
public:
    Before();
    void draw(QPainter &event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);// alt +回车
    void timerEvent(QTimerEvent *event);
    static int getRandomInt(int a,int b);
    Before *before = nullptr;
private:

};

#endif // BEFORE_H
