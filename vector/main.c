#include <stdlib.h>
#include <string.h>

struct vector {
    void* data;
    size_t size;
    size_t capacity;
    size_t bytes;
};

struct vector vector_create(size_t bytes)
{
    struct vector vector;
    vector.data = NULL;
    vector.size = 0;
    vector.capacity = 0;
    vector.bytes = bytes;
    return vector;
}

void vector_free(struct vector* vector)
{
    if (vector->data) {
        free(vector->data);
        vector->data = NULL;
        vector->size = 0;
        vector->capacity = 0;
    }
}

void* vector_push(struct vector* vector, const void* value)
{
    if (vector->size + 1 >= vector->capacity) {
        vector->capacity = (!vector->capacity + vector->capacity) * 2;
        vector->data = realloc(vector->data, vector->capacity * vector->bytes);
    }
    void* ptr = (char*)vector->data + vector->size++ * vector->bytes;
    memcpy(ptr, value, vector->bytes);
    return ptr;
}

#define SIZE 100

int printf(const char*, ...);

int main(void)
{
    size_t i;
    struct vector vector = vector_create(sizeof(double));
    for (i = 0; i < SIZE; ++i) {
        double n = (double)i;
        vector_push(&vector, &n);
    }

    double *arr = vector.data;
    for (i = 0; i < vector.size; ++i) {
        printf("%lf\n", arr[i]);
    }

    vector_free(&vector);
    return 0;
}


