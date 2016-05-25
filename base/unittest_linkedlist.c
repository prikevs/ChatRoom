#include "linkedlist.h"
#include <stdio.h>
#include <string.h>

void cleanData(void *data)
{
    printf("Clean %d\n", *(int*)data);
}

void showData(void *data)
{
    printf("%d", *(int*)data);
}

void test_1()
{
    int key_len = 2;
    LinkedList *list = initList(key_len);
    uint8_t key[2];
    key[0] = 0;
    key[1] = 3;
    int data1 = 3;
    addNodeToList(list, makeNode((void *)&data1, key, key_len));

    key[0] = 0;
    key[1] = 1;
    int data2 = 1;
    addNodeToList(list, makeNode((void *)&data2, key, key_len));

    key[0] = 0;
    key[1] = 2;
    int data3 = 2;
    addNodeToList(list, makeNode((void *)&data3, key, key_len));
    dumpList(list, showData);

    key[1] = 2;
    printf("Status: %d\n", deleteNode(list, key, cleanData));
    dumpList(list, showData);
    printf("findNode: %d\n", findNode(list, key)->data);
}

void test_deep_copy()
{
    int key_len = 2;
    LinkedList *list = initList(key_len);
    uint8_t key[2];
    key[0] = 0;
    key[1] = 1;
    int data1 = 1;
    addNodeToList(list, makeNode((void *)&data1, key, key_len));

    key[0] = 0;
    key[1] = 2;
    int data2 = 2;
    addNodeToList(list, makeNode((void *)&data2, key, key_len));
    dumpList(list, showData);

    LinkedList *new = deepCopyList(list, sizeof(int));
    dumpList(new, showData);
}
int main()
{
    test_deep_copy();
    return 0;
}
