#include "before.h"
#include "widget.h"
#include <QPainter>
#include <Qobject>
Before::Before() {
}

void Before::draw(QPainter &painter)
{

    painter.setPen(Qt::white);
    painter.setFont(QFont("宋体", 50));
    painter.drawText(120,100, "飞机大战" );

}
