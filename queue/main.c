#include <stdlib.h>
#include <string.h>

struct queue {
    void* data;
    size_t capacity;
    size_t rear;
    size_t front;
    size_t bytes;
};

static void queue_clear(struct queue* queue)
{
    queue->data = NULL;
    queue->rear = 0;
    queue->front = 0;
    queue->capacity = 0;
}

struct queue queue_create(size_t bytes)
{
    struct queue queue;
    queue_clear(&queue);
    queue.bytes = bytes;
    return queue;
}

void queue_free(struct queue* queue)
{
    if (queue->data) {
        free(queue->data);
        queue_clear(queue);
    }
}

size_t queue_count(const struct queue* queue)
{
    if (queue->rear <= queue->front) {
        return queue->front - queue->rear;
    }
    return queue->capacity - queue->rear + queue->front;
}


void* queue_push(struct queue* queue, const void* data)
{
    int wrapped = queue->front + 1 == queue->rear;
    size_t count = queue_count(queue);
    if (wrapped || count + 1 >= queue->capacity) {
        size_t currcap = queue->capacity;
        queue->capacity = (!queue->capacity + queue->capacity) * 2;
        queue->data = realloc(queue->data, queue->capacity * queue->bytes);
        if (wrapped) {
            memcpy(
                (char*)queue->data + currcap * queue->bytes,
                queue->data,
                queue->front * queue->bytes
            );
            queue->front += currcap;
        }
    }

    void* ptr = (char*)queue->data + queue->front * queue->bytes;
    queue->front = (queue->front + 1) % queue->capacity;
    memcpy(ptr, data, queue->bytes);
    return ptr;
}

void* queue_pop(struct queue* queue)
{
    void* ptr = NULL;
    if (queue->rear != queue->front) {
        ptr = (char*)queue->data + queue->rear * queue->bytes;
        queue->rear = (queue->rear + 1) % queue->capacity;
        if (queue->rear == queue->front) {
            queue->rear = 0;
            queue->front = 0;
        }
    }
    return ptr;
}

#define SIZE 1000
int printf(const char*, ...);

int main(void)
{
    size_t i, count;
    struct queue queue = queue_create(sizeof(double));
    for (i = 0; i < SIZE; ++i) {
        double n = (double)i;
        queue_push(&queue, &n);
    }

    count = queue_count(&queue) / 2;
    for (i = 0; i < count; ++i) {
        printf("%lf\n", *(double*)queue_pop(&queue));
    }

    for (i = 0; i < SIZE; ++i) {
        double n = (double)i;
        queue_push(&queue, &n);
    }

    while (queue.rear != queue.front) {
        printf("%lf\n", *(double*)queue_pop(&queue));
    }

    printf("Size: %zu\n", queue_count(&queue));
    printf("Capacity: %zu\n", queue.capacity);

    queue_free(&queue);
    return 0;
}

