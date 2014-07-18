/**
 * qtree.h
 * voronoi
 * May 5, 2012
 * Brandon Surmanski
 */

#ifndef _QTREE_H
#define _QTREE_H

#define QTREE_TOP_LEFT 0
#define QTREE_TOP_RIGHT 1
#define QTREE_BOTTOM_LEFT 2
#define QTREE_BOTTOM_RIGHT 3

#include <stdbool.h>

typedef struct QTreeEntry {
    float x;
    float y;
} QTreeEntry;

typedef struct QNode {
    bool leaf;
    struct QNode *parent;
    struct QNode *children[4];
    struct QTreeEntry *entryPtr;
} QNode;

typedef struct QTree {
   float sideLength;
   QNode root; 
} QTree;


void qtree_init(QTree *qtree, float sideLength);
void qtree_finalize(QTree *qtree);
float qtree_closest(QTree *tree, float x, float y);

void qtree_initEntry(QTreeEntry *e, float x, float y);
void qtree_moveEntry(QTreeEntry *e, float nx, float ny);
void qtree_addEntry(QTree *tree, QTreeEntry *e);
void qtree_removeEntry(QTree *tree, QTreeEntry *e);
float qtree_distanceToEntry(QTreeEntry *e, float x, float y);

#endif
