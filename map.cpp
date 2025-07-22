#include "map.h"
#include <QPixmap>
#include "config.h"
#include <QPainter>
Map::Map()
{
    reloadMap();
}

void Map::map_pos()
{

    map_1_y+=map_speed;
    map_2_y+=map_speed;
    if(map_1_y>=0){
        map_1_y=-WIN_H;
        map_2_y=0;
        }
}
void Map::draw(QPainter &painter)
{
    painter.drawPixmap(0,Map::map_1_y,Map::map_1);
    painter.drawPixmap(0,Map::map_2_y,Map::map_2);
}

void Map::reloadMap()
{
    map_1.load(":/new/res/img_bg_level_"+QString::number(level)+".jpg");
    map_2.load(":/new/res/img_bg_level_"+QString::number(level)+".jpg");
    map_1_y = -WIN_H;
    map_2_y =0;
    map_speed = MAP_SPEED;
}

