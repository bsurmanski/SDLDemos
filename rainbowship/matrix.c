#include <stdlib.h>
#include "matrix.h"
#include "trigtable.h"

TrigTable *t = 0;

inline void apply_matrix(Vector *v, Matrix *m)
{
    float x = v->x;
    float y = v->y;
    v->x = x * m->array[0][0] + y * m->array[1][0];
    v->y = x * m->array[0][1] + y * m->array[1][1];
}

Matrix *get_rotation_matrix(int degrees)
{
    if (!t){
        t = init_table();
    }
    
    Matrix *m = malloc(sizeof(Matrix));
    float sin = lookup_sin(t, degrees);
    float cos = lookup_cos(t, degrees);
    m->array[0][0] = cos;
    m->array[1][0] = -sin;
    m->array[0][1] = sin;
    m->array[1][1] = cos; 
    return m;
}

Vector *get_vector()
{
    Vector *v = malloc(sizeof(Vector));
    v->x = 0;
    v->y = 0;
    return v;
}

Vector *get_unit_vector(int rotation)
{
    Vector *v = malloc(sizeof(Vector));
    v->x = 0;
    v->y = -1;
    Matrix *m = get_rotation_matrix(360-rotation);
    apply_matrix(v, m);
    free(m);
    return v;
}

inline void set_vector(Vector *v, float x, float y)
{
    v->x = x;
    v->y = y;
}
