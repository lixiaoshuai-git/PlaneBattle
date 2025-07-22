#ifndef MAP_H
#define MAP_H
#include <QPixmap>
class Map
{
public:
    Map();
    void map_pos();
    void draw(QPainter &painter);
    QPixmap map_1;
    QPixmap map_2;
    int map_1_y;
    int map_2_y;
    int map_speed;
    int level=1;
    void reloadMap();
};

#endif // MAP_H
