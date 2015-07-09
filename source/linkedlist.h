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

ListNode *make_node_calloc(void *value);
void list_add_last(List *list, ListNode *node);
void list_add_first(List *list, ListNode *node);
void *list_remove(List *list, ListNode *node);
void *list_pop(List *list);
void *list_shift(List *list);

#endif
