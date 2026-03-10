#include "List.h"

void addToList(List **head, void *toAdd) {
  // Verif entry
  if (head == NULL || toAdd == NULL)
    return;

  // Create new list element
  List *newElem = (List *)malloc(sizeof(List));
  if (newElem == NULL) {
    printf("Error fn: addToList\n");
    return;
  }

  // Add it at first + Add Data Into
  newElem->next = *head;
  newElem->data = toAdd;

  *head = newElem;
}

void removeFromList(List **head, void *toAdd) {
  // Verif
  if (head == NULL || toAdd == NULL) {
    printf("Error fn: removeFromList\n");
    return;
  }

  // Verif First TODODODOOD

  // Search It TODOODOODODO
  List *tmp = *head;
}
