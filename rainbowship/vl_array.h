#include "string.h"

/*structs*/
typedef struct vl_array vl_array;

/*functions*/
void add_element(vl_array *array, void* element);

void remove_element(vl_array *array, void* element);

void* get_element(vl_array *array, int index);

size_t vl_array_length(vl_array *array);

vl_array *new_array(size_t element_size);

void delete_array(vl_array *array);

