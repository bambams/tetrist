#include <stdlib.h>
#include <string.h>

#include "list.h"

void list_free(LINKED_LIST ** plist) {
    list_destroy(plist, free_nil);
}

void list_destroy(LINKED_LIST ** plist, FUNCTION_DESTROY data_free) {
    while(*plist) {
        LINKED_LIST * list = *plist;
        LINKED_LIST * next = list->next;

        if(data_free != NULL && list->data != NULL) {
            data_free(&list->data);
        }

        free(list);
        *plist = next;
    }
}

LINKED_LIST * list_create_link(void * data) {
    LINKED_LIST * list = malloc(sizeof(LINKED_LIST));

    if(list == NULL) {
        return NULL;
    }

    memset(list, 0, sizeof(LINKED_LIST));

    list->data = data;

    return list;
}

int list_add(LINKED_LIST ** plist, void * data) {
    LINKED_LIST * list = *plist;

    if(list == NULL) {
        *plist = list_create_link(data);

        return *plist != NULL;
    }

    while(1) {
        LINKED_LIST ** pnext = &list->next;

        if(*pnext == NULL) {
            *pnext = list_create_link(data);

            return *pnext != NULL;
        }

        list = *pnext;
    }
}

void list_print(LINKED_LIST * list, LINKED_LIST_PRINT_FUNCTION print) {
    int i = 0;

    while(list != NULL) {
        print(i++, list->data);
    }
}

int list_remove(
        LINKED_LIST ** list,
        void * target,
        FUNCTION_DESTROY data_free) {
    if((*list)->data == target) {
        if(data_free != NULL && target != NULL) {
            data_free(&(*list)->data);
        }

        free(*list);
        *list = (*list)->next;
        return 1;
    }

    LINKED_LIST ** node = list;

    while(1) {
        node = &(*node)->next;

        if((*node)->data == target) {
            if(data_free != NULL && target != NULL) {
                data_free(&(*node)->data);
            }

            free(*node);
            *node = (*node)->next;
            return 1;
        }
    }

    return 0;
}
