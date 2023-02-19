#include <stdlib.h>
#include <string.h>

struct string {
    char* data;
    size_t size;
    size_t capacity;
};

char* string_push_len(struct string* string, const char* data, size_t len)
{
    if (!data) {
        return NULL;
    }

    if (string->size + len >= string->capacity) {
        string->capacity = string->capacity ? string->capacity << 1 : sizeof(void*);
        while (string->size + len >= string->capacity) {
            string->capacity <<= 1;
        }
        string->data = realloc(string->data, string->capacity);
    }

    if (string->data) {
        memcpy(string->data + string->size, data, len);
        string->size += len;
        string->data[string->size] = 0;
    }

    return string->data;
}

char* string_push(struct string* string, const char* data)
{
    return string_push_len(string, data, data ? strlen(data) : 0);
}

char* string_join(struct string* s1, const struct string* s2)
{
    return string_push_len(s1, s2->data, s2->size);
}

struct string string_create(const char* data)
{
    struct string string;
    string.data = NULL;
    string.size = 0;
    string.capacity = 0;
    string_push(&string, data);
    return string;
}

void string_free(struct string* string)
{
    if (string->data) {
        free(string->data);
        string->data = NULL;
        string->size = 0;
        string->capacity = 0;
    }
}

char* string_search(const struct string* string, const char* data)
{
    if (string->data) {
        const char* big = string->data;
        while (*big) {
            const char* a = big, *b = data;
            while (*a == *b) {
                if (!*++b) {
                    return (char*)(size_t)big;
                }
                ++a;
            }
            ++big;
        }
    }
    return NULL;
}

void string_erase(struct string* string, size_t index, size_t len)
{
    if (string->size > index) {
        memmove(
            string->data + index,
            string->data + index + len,
            string->size - (index + len) + 1
        );
        string->size -= len;
        string->data[string->size] = 0;
    }
}

int printf(const char*, ...);

int main(void)
{
    struct string s1 = string_create(NULL);
    struct string s2 = string_create("This is a pretty long string of characters");

    string_push(&s1, "Hello");
    string_push(&s1, ", ");
    string_push(&s1, "World");
    string_push(&s1, "! ");

    printf("%s\n", s1.data);
    printf("Join: %s\n", string_join(&s1, &s2));

    char* search = string_search(&s1, "string");
    printf("Search: string -> %s\n", search ? "true" : "false");

    string_erase(&s1, 0, 14);
    printf("%s\n", s1.data);

    string_free(&s1);
    string_free(&s2);
    return 0;
}

