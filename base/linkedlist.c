#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "linkedlist.h"

// 回调函数，用来判断ListNode 的key值大小
static int compareKey(uint8_t *key_a, uint8_t *key_b, size_t key_len)
{
    size_t i;
    for(i = 0; i < key_len; ++i) {
        if (key_a[i] < key_b[i]) 
            return -1;
        else if (key_a[i] > key_b[i])
            return 1;
    }
    return 0;
}

// 根据传入的参数创建一个ListNode
ListNode *makeNode(void *data, uint8_t *key, size_t key_len)
{
    ListNode *node;

    node = (ListNode *)calloc(1, sizeof(ListNode));
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    node->key = (uint8_t *)calloc(key_len, sizeof(uint8_t));
    memcpy(node->key, key, key_len);

    return node;
}

// 初始化链表
LinkedList* initList(size_t key_len)
{
    LinkedList *list;

    list  = (LinkedList *)calloc(1, sizeof(LinkedList));
    list->key_len = key_len;

    return list;
}

// 将一个节点加入链表
int addNodeToList(LinkedList *list, ListNode *node)
{
    ListNode *p, *prev;

    if (list->head == NULL) {
        list->head = node;
        return 0;
    }
    p = list->head;
    prev = list->head;

    while (p != NULL && compareKey(p->key, node->key, list->key_len) < 0) {
        prev = p;
        p = p->next;
    }

    if (p == list->head) {
        node->next = p; 
        p->prev = node;
        list->head = node;
        return 0;
    }

    if (p == NULL) {
        node->prev = prev;
        prev->next = node; 
        return 0;
    }

    p->prev->next = node;
    node->prev = p->prev;
    node->next = p;
    p->prev = node;
    return 0;
}

// 清除一个节点
void cleanNode(ListNode **node, void (* cleanData)(void *))
{
    cleanData((*node)->data);
    free((*node)->key);
    free(*node);
}

// 寻找一个节点
ListNode *findNode(LinkedList *list, uint8_t *key)
{
    ListNode *p;
    
    if (list->head == NULL) {
        return NULL;
    }
    p = list->head;
    while(p != NULL && compareKey(p->key, key, list->key_len) < 0)
        p = p->next;
    if (p == NULL || compareKey(p->key, key, list->key_len) != 0)
        return NULL;
    return p;
}

// 更新一个节点
int updateNode(LinkedList *list, uint8_t *key, void *data, void(* cleanData)(void *))
{
    ListNode *p;
    p = findNode(list, key);
    if (p == NULL)
        return -1;
    cleanData(p->data); 
    p->data = data;
    return 0;
}

// 根据传入的节点指针从链表中删除对应节点
int deleteNodeByNode(LinkedList *list, ListNode *p, void(* cleanData)(void *))
{
    if (p == list->head) {
        list->head = p->next; 
        if (p->next != NULL)
            p->next->prev = NULL;
    }
    else {
        p->prev->next = p->next;
        if (p->next != NULL)
            p->next->prev = p->prev;
    }
    cleanNode(&p, cleanData);
    return 0;
}

// 根据该节点的Key删除某节点
int deleteNode(LinkedList *list, uint8_t *key, void(* cleanData)(void *))
{
    ListNode *p;

    p = findNode(list, key);
    if (p == NULL)
        return -1;

    return deleteNodeByNode(list, p, cleanData);
}

// 深度复制某个节点
ListNode *copyNode(ListNode *src, size_t key_len, size_t void_len)
{
    void *data = calloc(void_len, 1); 
    memcpy(data, src->data, void_len);
    return makeNode(data, src->key, key_len);
}

// 深度复制某个链表
LinkedList *deepCopyList(LinkedList *src, size_t void_len)
{
    LinkedList *dst;
    ListNode *node, *newnode;
    
    dst = initList(src->key_len);
    if (src->head == NULL)
        return dst;
    dst->head = copyNode(src->head, src->key_len, void_len);
    node = src->head->next;
    newnode = dst->head;
    while(node != NULL) {
        newnode->next = copyNode(node, src->key_len, void_len); 
        newnode->next->prev = newnode;
        newnode = newnode->next;
        node = node->next;
    }
    return dst; 
}

// 删除某个链表并释放所有资源
void destroyList(LinkedList **list, void(* cleanData)(void *))
{
    ListNode *p, *temp;
    p = (*list)->head;
    while(p != NULL) {
        temp = p;
        p = p->next; 
        cleanNode(&temp, cleanData);
    }
    free(*list);
    *list = NULL; 
}

// 打印Key，调试用
void showKey(uint8_t* key, size_t key_len)
{
    //int i;
    // for(i = 0; i < key_len; ++i)
        //printf("%2d ", key[i]); 
    printf("%s ", key);
}

// 显示整个链表
void dumpList(const LinkedList *list, void(* showData)(void *))
{
    const ListNode *node;
    node = list->head;
    while(node != NULL) {
        printf("key:");
        showKey(node->key, list->key_len);
        if (node->prev == NULL)
            printf("prev->NULL ");
        else
            printf("prev->HAVE ");
        if (node->next == NULL)
            printf("next->NULL ");
        else
            printf("next->HAVE ");
        printf(" data:");
        showData(node->data);
        printf("\n");
        node = node->next;
    }
}
