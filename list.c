#include <stdlib.h>
#include <string.h>

#include "list.h"

void free_nil(void ** data) {}

void list_free(LINKED_LIST ** plist) {
    list_destroy(plist, free_nil);
}

void list_destroy(LINKED_LIST ** plist, void (*data_free)(void **)) {
    while(*plist) {
        LINKED_LIST * list = *plist;
        LINKED_LIST * next = list->next;

        if(list->data) {
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

int list_remove(LINKED_LIST ** plist, LINKED_LIST * element) {
    LINKED_LIST * list = *plist;

    if(list == NULL) {
        return 0;
    }

    while(1) {
        LINKED_LIST ** pnext = &list->next;

        if(*pnext == NULL) {
            return 0;
        }

        if(*pnext == element) {
            *pnext = NULL;

            pnext = &element->next;

            if(*pnext != NULL) {
                list->next = *pnext;
            }

            return 1;

        }

        list = *pnext;
    }
}