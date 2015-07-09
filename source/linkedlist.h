#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;        \
    ListNode *V = NULL;                                         \
    for (V = _node = L->S; _node != NULL; V = _node = _node->M) \

typedef struct ListNode
{
    struct ListNode *next;
    struct ListNode *prev;
    void* value;
} ListNode;

typedef struct List
{
    ListNode *first;
    ListNode *last;
    int length;
} List;

#endif
