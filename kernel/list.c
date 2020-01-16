#include "list.h"

inline int link_length(struct link_head *head)
{
    int i = 0;
    link_foreach(head, p)
        i++;
    return i;
}


// prepend a node to link
inline struct link_head* link_prepend(struct link_head *head, struct link_head *node)
{
    node->prev = head;
    node->next = head->next;
    head->next->prev = node;
    head->next = node;
    return node;
}

// append a node to link
inline struct link_head* link_append(struct link_head *head, struct link_head *node)
{
    node->next = head;
    node->prev = head->prev;
    head->prev->next = node;
    head->prev = node;
    return node;
}

// remove node
// return: pointer to removed node if succeed, or NULL.
inline struct link_head* link_remove(struct link_head *head, struct link_head *node)
{
    if (link_isempty(head) || link_isempty(node))
        return NULL;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = node->next = NULL;
    return node;
}
