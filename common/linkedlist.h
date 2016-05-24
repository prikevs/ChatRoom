#ifndef LINKEDLIST_H
#define LINKEDLIST
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

typedef struct ListNode {
    void *data;
    uint8_t *key;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

typedef struct LinkedList {
    ListNode *head;
    size_t key_len;
} LinkedList;

//int compareKey(uint8_t*, uint8_t*, size_t);

ListNode* makeNode(void*, uint8_t*, size_t);
LinkedList* initList(size_t);
int addNodeToList(LinkedList*, ListNode*);
void cleanNode(ListNode**, void(*)(void*));
ListNode* findNode(LinkedList *list, uint8_t *key);
int deleteNode(LinkedList*, uint8_t*, void(*)(void*));
void destroyList(LinkedList **list, void(*)(void*));
int updateNode(LinkedList *, uint8_t *, void *, void(*)(void *));

#endif
