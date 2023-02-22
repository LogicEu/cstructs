#include <stdlib.h>
#include <string.h>

struct treenode {
    struct treenode** children;
    struct treenode* parent;
    void* data;
};

size_t treenode_children_count(const struct treenode* node)
{
    size_t i;
    for (i = 0; node->children[i]; ++i);
    return i;
}

size_t treenode_children_capacity(const size_t children_count)
{
    size_t i;
    for (i = 1; i < children_count; i <<= 1);
    return i;
}

struct treenode* treenode_create(const void* data, size_t bytes)
{
    struct treenode* node = malloc(sizeof(struct treenode) + bytes);
    node->children = calloc(sizeof(struct treenode*), 1);
    node->parent = NULL;
    node->data = node + 1;
    memcpy(node->data, data, bytes);
    return node;
}

void treenode_free(struct treenode* node)
{
    size_t i;
    for (i = 0; node->children[i]; ++i) {
        treenode_free(node->children[i]);
    }

    free(node->children);
    free(node);
}

struct treenode* treenode_push(struct treenode* node, struct treenode* child)
{
    const size_t count = treenode_children_count(node);
    const size_t cap = treenode_children_capacity(count + 1);

    if (count + 2 >= cap) {
        node->children = realloc(node->children, cap * 2 * sizeof(struct treenode*));
    }

    if (node->children) {
        child->parent = node;
        node->children[count] = child;
        node->children[count + 1] = NULL;
        return child;
    }
    return NULL;
}

void treenode_remove(struct treenode* node, const size_t index)
{
    const size_t count = treenode_children_count(node);
    if (index < count) {
        treenode_free(node->children[index]);
        memmove(
            node->children + index,
            node->children + index + 1,
            (count - index + 1) * sizeof(struct treenode*)
        );
    }
}

int printf(const char*, ...);

void tree_print(const struct treenode* node, const size_t currdepth)
{
    size_t i;
    for (i = 0; i < currdepth; ++i) {
        printf("   ");
    }

    printf("|-> %zu\n", *(size_t*)node->data);

    for (i = 0; node->children[i]; ++i) {
        tree_print(node->children[i], currdepth + 1);
    }
}

struct treenode* tree_spawn(const size_t maxdepth, const size_t currdepth)
{
    struct treenode* node = treenode_create(&currdepth, sizeof(currdepth));
    if (currdepth < maxdepth) {
        size_t i;
        for (i = 0; i < maxdepth - currdepth; ++i) {
            struct treenode* child = tree_spawn(maxdepth, currdepth + 1);
            treenode_push(node, child);
        }
    }
    return node;
}

int main(void)
{
    struct treenode* root = tree_spawn(3, 0);
    tree_print(root, 0);
    treenode_remove(root, 0);
    tree_print(root, 0);
    treenode_free(root);
    return 0;
}


