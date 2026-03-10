#ifndef List_H
#define List_H

#include <stdlib.h>
#include <stdio.h>

typedef struct list {
  void *data;
  struct list *next;
} List;

void addToList(List **head, void *toAdd);
void removeFromList(List **head, void *toAdd);

#endif
