/**
 * qtree.c
 * voronoi
 * May 5, 2012
 * Brandon Surmanski
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#include "qtree.h"

static void qnode_init(QNode *n, QNode *parent)
{
    int i;
    n->leaf = true;
    n->parent = parent;
    for(i = 0; i < 4; i++)
        n->children[i] = NULL;
    n->entryPtr = NULL;
}

static void qnode_finalize(QNode *n)
{
    int i;
    n->parent = NULL;
    for(i = 0; i < 4; i++){
        if(n->children[i]){
            qnode_finalize(n->children[i]);
            free(n->children[i]);
        }
        n->entryPtr = NULL; 
    }
}

static int qnode_classifyEntry(QTreeEntry *e, float cx, float cy)
{
    if(e->y > cy){
        if(e->x > cx)
            return QTREE_TOP_RIGHT;
        return QTREE_TOP_LEFT;
    } else {
        if(e->x > cx)
            return QTREE_BOTTOM_RIGHT;
        return QTREE_BOTTOM_LEFT;
    }
}

static void qnode_childCenter(int classify, 
        float cx, float cy, 
        float childSideLen, 
        float *ccx, float *ccy) {
        switch(classify) {
            case QTREE_TOP_LEFT:
                *ccx = cx - childSideLen/2.0f;
                *ccy = cy + childSideLen/2.0f;
                break;
            case QTREE_TOP_RIGHT:
                *ccx = cx + childSideLen/2.0f;
                *ccy = cy + childSideLen/2.0f;
                break;
            case QTREE_BOTTOM_LEFT:
                *ccx = cx - childSideLen/2.0f;
                *ccy = cy - childSideLen/2.0f;
                break;
            case QTREE_BOTTOM_RIGHT:
                *ccx = cx + childSideLen/2.0f;
                *ccy = cy - childSideLen/2.0f;
                break;
        }
}

static void qnode_split(QNode *n)
{
    int i;
    int cnum;

    for(i = 0; i < 4; i++){
        n->leaf = false;
        n->children[i] = malloc(sizeof(QNode));
        qnode_init(n->children[i], n);
    }
}



QTreeEntry *qnode_closest(QNode *n, float x, float y, float cx, float cy, float sideLen, int ind)
{
    int i;
    QTreeEntry *closest = 0;
    float distance = 100000;
    QTreeEntry *tmp = 0;
    float tmpDistance = 0;
    if(n->leaf){
        if(n->entryPtr){
            closest = n->entryPtr;
            distance = qtree_distanceToEntry(closest, x, y);
        } 
        if(ind) return closest;
        for(i = 0; i < 4; i++){
            if (n->parent->children[i] != n){
                //OK, I just started getting lazy here...
                float csl = sideLen;
                float ccx = (x > cx ? cx + csl : cx - csl); 
                float ccy = (y > cy ? cy + csl : cy - csl);
                tmp = qnode_closest(n->parent->children[i], x, y, ccx, ccy, csl, 1);
                if(!tmp) continue;
                tmpDistance = qtree_distanceToEntry(tmp, x, y);
                if(tmpDistance < distance){
                    closest = tmp;
                    distance = tmpDistance;
                }
            }
        }  
        return closest;
    } else {
        float childSide = sideLen / 2.0f;
        if(y > cy){
            if(x > cx){
                return qnode_closest(n->children[QTREE_TOP_RIGHT], 
                        x, y, cx + childSide/2.0f, cy + childSide/2.0f, childSide, 0);
            } else {
                return qnode_closest(n->children[QTREE_TOP_LEFT], 
                    x, y, cx - childSide/2.0f, cy + childSide/2.0f, childSide, 0);
            }
        } else {
            if(x > cx){
                return qnode_closest(n->children[QTREE_BOTTOM_RIGHT], 
                        x, y, cx + childSide/2.0f, cy + childSide/2.0f, childSide, 0);
            } else {
                return qnode_closest(n->children[QTREE_BOTTOM_LEFT], 
                    x, y, cx - childSide/2.0f, cy + childSide/2.0f, childSide, 0);
            }
        } 
    }
    return 0;
}

static void qnode_addEntry(QNode *n, QTreeEntry *e, float cx, float cy, float sideLen)
{
    int cnum;
    float childSideLen = sideLen / 2.0f;
    float cxChild;
    float cyChild;

    if(n->leaf){
        if(n->entryPtr){
            qnode_split(n);

            cnum = qnode_classifyEntry(n->entryPtr, cx, cy);
            qnode_childCenter(cnum, cx, cy, childSideLen, &cxChild, &cyChild);
            qnode_addEntry(n->children[cnum], n->entryPtr, cxChild, cyChild, childSideLen);

            cnum = qnode_classifyEntry(e, cx, cy);
            qnode_childCenter(cnum, cx, cy, childSideLen, &cxChild, &cyChild);
            qnode_addEntry(n->children[cnum], e, cxChild, cyChild, childSideLen);
            return;
        } else {
            n->entryPtr = e;
        }
    } else {
            cnum = qnode_classifyEntry(e, cx, cy);
            qnode_childCenter(cnum, cx, cy, childSideLen, &cxChild, &cyChild);
            qnode_addEntry(n->children[cnum], e, cxChild, cyChild, childSideLen);
            return;
    }
}

static void qnode_removeEntry(QNode *n, QTreeEntry *e)
{

}

void qtree_init(QTree *qtree, float sideLength)
{
    qtree->sideLength = sideLength;
    qnode_init(&(qtree->root), NULL);
}

void qtree_finalize(QTree *qtree)
{
    qnode_finalize(&(qtree->root)); 
}

float qtree_closest(QTree *tree, float x, float y)
{
    QTreeEntry *e = qnode_closest(&(tree->root), x, y, 
                tree->sideLength / 2.0f, tree->sideLength / 2.0f, tree->sideLength, 0);
    if(!e) return 1000;
    return qtree_distanceToEntry(e,x,y); 
}

void qtree_initEntry(QTreeEntry *e, float x, float y)
{
    
}

void qtree_moveEntry(QTreeEntry *e, float x, float y)
{

}

void qtree_addEntry(QTree *tree, QTreeEntry *e)
{
    float len = tree->sideLength;
   qnode_addEntry(&(tree->root), e, len / 2.0f, len / 2.0f, len);
}

void qtree_removeEntry(QTree *tree, QTreeEntry *e)
{
    qnode_removeEntry(&(tree->root), e);
}

float qtree_distanceToEntry(QTreeEntry *e, float x, float y)
{
    float dx = e->x - x;
    float dy = e->y - y;
    return sqrt(dx * dx + dy * dy);
}
