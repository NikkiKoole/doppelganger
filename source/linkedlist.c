#include "linkedlist.h"

ListNode *make_node_calloc(void *value) {
    ListNode *node = calloc(1, sizeof(ListNode));
    node->value = value;
    return node;
}

int list_add_last(List *list, ListNode *node)
{
    //ListNode *node = calloc(1, sizeof(ListNode));
    //node->value = value;
    if (list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->prev = list->last;
        list->last->next = node;
        list->last = node;
    }
    list->length++;
}
int list_add_first(List *list, ListNode *node)
{
    //ListNode *node = calloc(1, sizeof(ListNode));
    //node->value = value;
    if (list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }
    list->length++;
}

void *list_remove(List *list, ListNode *node)
{
    void *result;
    ASSERT(list->first && list->last && "List cannot be empty.");
    ASSERT(node && "Node cannot be null.");

    if (node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if (node == list->first) {
        list->first = node->next;
        ASSERT(list->first != NULL && "invalid list; first became null somehow.");
        list->first->prev = NULL;
    } else if (node == list->last) {
        list->last = node->prev;
        ASSERT(list->last != NULL && "invalid list; last became null somehow.");
        list->last->next = NULL;
    } else {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }
    list->length--;
    result = node->value;
    return result;
}

void *list_pop(List *list)
{
    ListNode *node = list->last;
    return node != NULL ? list_remove(list, node) : NULL;
}
void *list_shift(List *list)
{
    ListNode *node = list->first;
    return node != NULL ? list_remove(list, node) : NULL;
}
