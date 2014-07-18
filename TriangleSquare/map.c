/*
 * map.c
 * glModern
 * December 30, 2011
 * Brandon Surmanski
 */

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

int w = 0;
int h = 0;
float maxz = 1;
float *data = 0;

static float heightFunc(int d, float h)
{
    return h + maxz * (((float) rand() / (float)RAND_MAX)-0.5) * d / 16;
}

float diamond(int x, int y, int d)
{
    float sum = 0;
    int num = 0;

    if(x-d >= 0) sum += data[x-d + w * y], num++;
    if(x+d < w) sum += data[x+d + w * y], num++;
    if(y-d >= 0) sum += data[x + w * (y-d)], num++;
    if(y+d < h) sum += data[x + w * (y+d)], num++;

    return heightFunc(d, sum/(float)num);
}

float square(int x, int y, int d)
{
    float sum = 0;
    int num = 0;

    if(x-d >= 0 && y-d >=0) sum += data[x-d + w * (y-d)], num++;
    if(x+d < w && y-d >=0) sum += data[x+d + w * (y-d)], num++;
    if(x-d >= 0 && y+d < h) sum += data[x-d + w * (y+d)], num++;
    if(x+d < w && y+d < h) sum += data[x+d + w * (y+d)], num++;

    return heightFunc(d, sum/(float)num);
}

void map_initDiamondSquare(int width, int height)
{
    int i, j;
    int d; // depth
    float sum = 0; //for averaging surrounding values
    int num = 0; //number of values averaged
    int index = 0;

    w = width;
    h = height;

    if(w != h){
        w = h = (w > h ? w : h); // assign both to max
    }

    if(w & (w-1) != 0){ // not power of 2
        for(i = 1; i < sizeof(int); i<<=1){
            w = w | w >> i;
        }
        h = w = w + 2; // set width and height to higher pow 2
    }

    if(data){
        free(data);
    }
    data = malloc(sizeof(float) * w * h);
    memset(data, 0, sizeof(float) * w * h);

    d = w; // delta distance for diamond square

    //init corners
    data[0 + (w * 0)] = heightFunc(d, 0.5f);
    data[0 + (w * d)] = heightFunc(d, 0.5f);
    data[d + (w * 0)] = heightFunc(d, 0.5f);
    data[d + (w * d)] = heightFunc(d, 0.5f);

    d/=2;
    while(d >= 1)
    {
        //square
        for(j = d; j < h; j+= 2*d){
            for(i = d; i < w; i+= 2*d){
                data[i + (w * j)] = square(i, j, d);
            }
        }
        //diamond
        for(j = d; j < h; j+= 2*d){
            for(i = 0; i < w; i+= 2*d){
                data[i + (w * j)] = diamond(i, j, d);
            }
        }

        for(j = 0; j < h; j+= 2*d){
            for(i = d; i < w; i+= 2*d)
            {
                data[i + (w * j)] = diamond(i, j, d);
            }
        }
        d/=2;
    }
}

float *map_getData()
{
    return data;
}

int map_getWidth()
{
    return w;
}

int map_getHeight()
{
    return h;
}

void map_setMaxHeight(float z)
{
    maxz = z;
}

