#include <stdlib.h>
#include "linkedlist.h"


/* push new generic node onto the top of the stack */

int pushState(LinkedListNode **head, void *newData) {

    int success;
    LinkedListNode *newNode;
    
    newNode = (LinkedListNode *)malloc(sizeof(LinkedListNode));
    success = 0;
    
    /* ensure memory alocation has succeeded before linking new node*/
    if (newNode != NULL) {
        newNode->data = newData;
        newNode->next = *head;
        *head = newNode;
        success = 1;
    }
    return success;
}

/* Remove top node from the stack  and return the data pointer*/
void *popState(LinkedListNode **head) {
    void *poppedData;
    LinkedListNode *tempNode;
    
    poppedData = NULL;

    /*Ensure the stack and pointer are not empty*/
    if (head != NULL && *head != NULL) {
        tempNode = *head;
        poppedData = tempNode->data;
        *head = tempNode->next;
        free(tempNode);
    }
    return poppedData;
}

/*Free  nodes */
void freeList(LinkedListNode **head, void (*freeDataFunc)(void *)) {
    LinkedListNode *current;
    LinkedListNode *nextNode;
    
    current = *head;
    
    /* Traverse through each neode in the list to free them one by one */
    while (current != NULL) {
        nextNode = current->next;

        if (freeDataFunc != NULL && current->data != NULL) {
            freeDataFunc(current->data);
        }
        free(current);
        current = nextNode;
    }
    *head = NULL;
}