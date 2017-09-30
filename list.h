#ifndef LIST_H
    #define LIST_H

struct LINKED_LIST {
    void * data;
    struct LINKED_LIST * next;
};

typedef struct LINKED_LIST LINKED_LIST;

void free_nil(void **);
void list_free(LINKED_LIST **);
void list_destroy(LINKED_LIST **, void (*)(void **));
LINKED_LIST * list_create_link(void *);
int list_add(LINKED_LIST **, void *);
int list_remove(LINKED_LIST **, void *);

#endif
