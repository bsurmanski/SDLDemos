#include <stdlib.h>
#include <math.h>
#include <float.h>
#define PI 3.141592654

typedef struct TrigTable {
    float *relations; 
} TrigTable;

TrigTable *init_table()
{
    int i;
    TrigTable *t = malloc(sizeof(TrigTable));
    t->relations = malloc(180 * sizeof(float));
    for (i = 0; i < 180; i++){
        t->relations[i] = sin(i * PI / 180.0);
    }
    return t;
}

float lookup_sin(TrigTable *t, float val)
{
    val = fmod(val, 360);
    int in = ((int)floor(val)) % 180;
    float ret = t->relations[in];
    if (val > 180){
    ret = -ret;
    } 
    return ret;
}

float lookup_cos(TrigTable *t, float val)
{
    return lookup_sin(t, val + 90);
}

float lookup_tan(TrigTable *t, float val)
{
    float cos = lookup_cos(t, val);
    if (cos == 0){
        return FLT_MAX;
    }
    float sin = lookup_sin(t, val);
    return sin/cos;
}
