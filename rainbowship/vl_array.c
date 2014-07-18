#include "vl_array.h"
#include <stdlib.h>
#include <math.h>

struct vl_array {
    size_t element_size;
    int length;
    int max;
    void* data;
};

void grow(vl_array *array)
{
    array->max = array->max * 2.0;
    array->data = realloc(array->data, array->max * array->element_size);
}

void shrink(vl_array *array)
{
    array->max = (int) ceil(array->max / 2.0);
    array->data = realloc(array->data, array->max * array->element_size);
}

int find_element(vl_array *array, void* element){
    int ret = -1;
    int i;
    for (i = 0; i < array->length; i++){
        if (memcmp(get_element(array, i), element, array->element_size) == 0){
            ret = i;
            break;
        }
    }
    return ret;
}

void add_element(vl_array *array, void* element)
{
    if (array->length + 1 == array->max)
        grow(array);
    memcpy(array->data + array->length * array->element_size, element, array->element_size); 
    ++(array->length);
}

void remove_element(vl_array *array, void* element)
{
    int i;
    for (i = find_element(array, element); i < array->length - 1; i++){
        void* element = get_element(array, i);
        memcpy(element, element + array->element_size, array->element_size); 
    } 
    --(array->length); 
    if (array->max > 10 && array->length < array->max/2)
        shrink(array);
}

void* get_element(vl_array *array, int index)
{
    return array->data + index * array->element_size;
}

size_t vl_array_length(vl_array *array)
{
    return array->length;
}

vl_array *new_array(size_t element_size)
{
    int max = 10;
    vl_array *a = malloc(sizeof(vl_array));
    a->element_size = element_size;
    a->length = 0;
    a->max = max;
    a->data = malloc(max*element_size);
    memset(a->data, 0x00000000, max*element_size);
    return a;
}

void delete_array(vl_array *array)
{
    free(array->data);
    free(array);
}

