#include <stdlib.h>
#include <string.h>

struct stack {
    void* data;
    size_t size;
    size_t capacity;
    size_t bytes;
};

struct stack stack_create(size_t bytes)
{
    struct stack stack;
    stack.data = NULL;
    stack.size = 0;
    stack.capacity = 0;
    stack.bytes = bytes;
    return stack;
}

struct stack stack_reserve(size_t size, size_t bytes)
{
    struct stack stack;
    stack.data = malloc(size * bytes);
    stack.size = size;
    stack.capacity = size;
    stack.bytes = bytes;
    return stack;
}

struct stack stack_wrap(void* data, size_t size, size_t bytes)
{
    struct stack stack;
    stack.data = data;
    stack.size = size;
    stack.capacity = size;
    stack.bytes = bytes;
    return stack;
}

void stack_free(struct stack* stack)
{
    if (stack->data) {
        free(stack->data);
        stack->data = NULL;
        stack->size = 0;
        stack->capacity = 0;
    }
}

void stack_clear(struct stack* stack)
{
    stack->size = 0;
}

void* stack_push(struct stack* stack, const void* data)
{
    if (stack->size + 1 >= stack->capacity) {
        stack->capacity = (!stack->capacity + stack->capacity) * 2;
        stack->data = realloc(stack->data, stack->capacity * stack->bytes);
    }

    void* ptr = (char*)stack->data + stack->size++ * stack->bytes;
    memcpy(ptr, data, stack->bytes);
    return ptr;
}

void* stack_pop(struct stack* stack)
{
    if (stack->size) {
        return (char*)stack->data + --stack->size * stack->bytes;
    }
    return NULL;
}

#define SIZE 1000
int printf(const char*, ...);

int main(void)
{
    size_t i, size;
    void* data = malloc(sizeof(double) * SIZE);
    struct stack stack = stack_wrap(data, SIZE, sizeof(double));
    for (i = 0; i < SIZE; ++i) {
        double n = (double)i;
        stack_push(&stack, &n);
    }

    size = stack.size / 2;
    for (i = 0; i < size; ++i) {
        printf("%lf\n", *(double*)stack_pop(&stack));
    }

    stack_clear(&stack);
    for (i = 0; i < SIZE; ++i) {
        double n = (double)i;
        stack_push(&stack, &n);
    }

    while (stack.size) {
        printf("%lf\n", *(double*)stack_pop(&stack));
    }

    stack_free(&stack);
    return 0;
}

