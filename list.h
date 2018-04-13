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
