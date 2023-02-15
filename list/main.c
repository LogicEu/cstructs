#include <stdlib.h>
#include <string.h>

struct node {
    void* data;
    struct node* next;
};

struct list {
    struct node* head;
    size_t bytes;
};

struct node* node_create(const void* data, const size_t bytes)
{
    struct node* node = malloc(sizeof(struct node) + bytes);
    node->data = node + 1;
    node->next = NULL;
    memcpy(node->data, data, bytes);
    return node;
}

struct list list_create(const size_t bytes)
{
    struct list list;
    list.head = NULL;
    list.bytes = bytes;
    return list;
}

size_t list_size(const struct list* list)
{
    size_t i = 0;
    struct node* node = list->head;
    while (node) {
        node = node->next;
        ++i;
    }
    return i;
}

void list_free(struct list* list)
{
    struct node* node = list->head;
    while (node) {
        struct node* next = node->next;
        free(node);
        node = next;
    }
}

void* list_push(struct list* list, const void* data)
{
    struct node* node = list->head;
    struct node* new_node = node_create(data, list->bytes);
    if (!node) {
        list->head = new_node;
    } else {
        while (node->next) {
            node = node->next;
        }
        node->next = new_node;
    }
    return new_node->data;
}

void list_remove(struct list* list, const size_t index)
{
    size_t i = 1;
    struct node* node = list->head, *prev;
    if (!node) {
        return;
    }

    if (!index) {
        list->head = node->next;
        free(node);
        return;
    }

    prev = node;
    node = node->next;
    while (node) {
        if (i == index) {
            prev->next = node->next;
            free(node);
            break;
        }
        prev = node;
        node = node->next;
        ++i;
    }
}

#define SIZE 1000
int printf(const char*, ...);

int main(void)
{
    size_t i, size;
    struct list list = list_create(sizeof(double));
    for (i = 0; i < SIZE; ++i) {
        double n = (double)i;
        list_push(&list, &n);
    }

    size = list_size(&list);
    for (i = 0; i < size; ++i) {
        if (i % 3 == 0) {
            list_remove(&list, i);
            --size;
        }
    }

    struct node* node = list.head;
    while (node) {
        printf("%lf\n", *(double*)node->data);
        node = node->next;
    }

    printf("size: %zu\n", list_size(&list));

    list_free(&list);
    return 0;
}


