/**
 * map.h
 * glModern
 * December 30, 2011
 * Brandon Surmanski
 */

#ifndef _MAP_H
#define _MAP_H

void map_initDiamondSquare(int w, int h);
float *map_getData();
int map_getWidth();
int map_getHeight();
void map_setMaxHeight(float z);

#endif
