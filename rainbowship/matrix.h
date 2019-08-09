typedef struct Matrix{
    float array[2][2];
} Matrix;

typedef struct Vector {
    float x;
    float y;
} Vector;

void apply_matrix(Vector *v, Matrix *m);
Matrix *get_rotation_matrix(int degrees);
Vector *get_vector();
Vector *get_unit_vector(int rotation);
void set_vector(Vector *v, float x, float y);
