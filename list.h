#ifndef LIST_H
    #define LIST_H

#include "misc.h"

struct LINKED_LIST {
    void * data;
    struct LINKED_LIST * next;
};

typedef struct LINKED_LIST LINKED_LIST;
typedef void (*LINKED_LIST_PRINT_FUNCTION)(int, void *);

int list_add(LINKED_LIST **, void *);
LINKED_LIST * list_create_link(void *);
void list_free(LINKED_LIST **);
void list_destroy(LINKED_LIST **, FUNCTION_DESTROY);
void list_print(LINKED_LIST *, LINKED_LIST_PRINT_FUNCTION);
int list_remove(LINKED_LIST **, void *, FUNCTION_DESTROY);

#endif
