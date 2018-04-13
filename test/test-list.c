//
// Tetrist is a falling block game.
// Copyright (C) 2018 Brandon McCaig
//
// This file is part of Tetrist.
//
// Tetrist is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Tetrist is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libbam.  If not, see <http://www.gnu.org/licenses/>.
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

typedef int (*PREDICATE)(LINKED_LIST *, LINKED_LIST *, void *);

int * create_int(int);
void free_int(int **);
int find_int(LINKED_LIST *, LINKED_LIST *, void *);
LINKED_LIST * list_find(LINKED_LIST *, PREDICATE, void *);
int test1(void);
int test2(void);

int main(int argc, char * argv[])
{
    int ret = 0;

    ret = test1();

    if(ret != 0) {
        return ret;
    }

    ret = test2();

    if(ret != 0) {
        return ret;
    }

    return 0;
}

int * create_int(int v) {
    int * i = malloc(sizeof(int));

    if(i != NULL) {
        *i = v;
    }

    return i;
}

int find_int(LINKED_LIST * list, LINKED_LIST * node, void * needle) {
    return *(int *)node->data == (int)needle;
}

void free_int(int ** pi) {
    assert(pi);

    if(*pi != NULL) {
        free(*pi);
        *pi = NULL;
    }
}

LINKED_LIST * list_find(LINKED_LIST * list,
                        PREDICATE predicate,
                        void * needle) {
    LINKED_LIST * node = list;

    while(node != NULL) {
        if(predicate(list, node, needle)) {
            return node;
        }

        node = node->next;
    }

    return NULL;
}

int test1(void) {
    fprintf(stderr, "test1: Start\n");

    LINKED_LIST * list = NULL;

    if(!list_add(&list, (void *)1)) {
        goto nomem;
    }

    assert(list->data == (void *)1);
    assert(list->next == NULL);

    if(!list_add(&list, (void *)2)) {
        goto nomem;
    }

    assert(list->data == (void *)1);
    assert(list->next->data == (void *)2);
    assert(list->next->next == NULL);

    if(!list_add(&list, (void *)3)) {
        goto nomem;
    }

    assert(list->data == (void *)1);
    assert(list->next->data == (void *)2);
    assert(list->next->next->data == (void *)3);
    assert(list->next->next->next == NULL);

    if(!list_remove(&list, (void *)2, (FUNCTION_DESTROY)free_nil)) {
        fprintf(stderr,
                "test1: Failed to remove the needle from the haystack.\n");
        goto error;
    }

    assert(list->data == (void *)1);
    assert(list->next->data == (void *)3);
    assert(list->next->next == NULL);

    list_free(&list);

    assert(list == NULL);

    fprintf(stderr, "test1: End\n");

    return 0;

nomem:
    fprintf(stderr, "test1: Out of memory.\n");
    goto error;

error:
    fprintf(stderr, "test1: Error.\n");
    list_free(&list);
    return 1;
}

int test2(void) {
    fprintf(stderr, "test2: Start\n");

    LINKED_LIST * list = NULL;
    int * i = create_int(1);

    if(i == NULL) {
        goto nomem;
    }

    if(!list_add(&list, i)) {
        goto nomem;
    }

    assert(*(int *)list->data == 1);
    assert(list->next == NULL);

    i = create_int(2);

    if(i == NULL) {
        goto nomem;
    }

    if(!list_add(&list, i)) {
        goto nomem;
    }

    assert(*(int *)list->data == 1);
    assert(*(int *)list->next->data == 2);
    assert(list->next->next == NULL);

    i = create_int(3);

    if(i == NULL) {
        goto nomem;
    }

    if(!list_add(&list, i)) {
        goto nomem;
    }

    assert(*(int *)list->data == 1);
    assert(*(int *)list->next->data == 2);
    assert(*(int *)list->next->next->data == 3);
    assert(list->next->next->next == NULL);

    LINKED_LIST * match = list_find(list, find_int, (void *)2);

    if(match == NULL) {
        fprintf(stderr,
                "test2: Failed to find the needle in the haystack.\n");
        goto error;
    }

    if(!list_remove(&list, match->data, (FUNCTION_DESTROY)free_int)) {
        fprintf(stderr,
                "test2: Failed to remove the needle from the haystack.\n");
        goto error;
    }

    assert(*(int *)list->data == 1);
    assert(*(int *)list->next->data == 3);
    assert(list->next->next == NULL);

    list_destroy(&list, (void (*)(void **))free_int);

    assert(list == NULL);

    fprintf(stderr, "test2: End\n");

    return 0;

nomem:
    fprintf(stderr, "test2: Out of memory.\n");
    goto error;

error:
    fprintf(stderr, "test2: Error.\n");
    list_destroy(&list, (void (*)(void **))free_int);
    free(i);
    return 1;
}
