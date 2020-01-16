#ifndef LIST_H
#define LIST_H

#ifndef NULL
#define NULL (void *) 0
#endif

/* 循环双向链表*/

struct link_head {
    struct link_head *prev, *next;
};

struct link_head* link_prepend(struct link_head *head, struct link_head *node);
struct link_head* link_append(struct link_head *head, struct link_head *node);
struct link_head* link_remove(struct link_head *head, struct link_head *node);
int link_length(struct link_head *head);

#define offsetof(type, field) \
    ((void *)(&((type *)0)->field))

#define link_init(head) \
    do { \
        (head)->next = (head); \
        (head)->prev = (head); \
    } while (0)

// EXPRESSION
// get pointer of structure which contains list_node `nodeptr`.
// `field` is the list_node field in structure.
#define link_entry(nodeptr, type, field) \
    ((type *)((void *)(nodeptr) - offsetof(type, field)))

// EXPRESSION
// check if the list pointed by `head` is empty
#define link_isempty(head) \
    ((head) == NULL || (head)->prev == NULL || (head)->next == NULL || \
    (head)->prev == (head) || (head)->next == (head))

// NOT EXPRESSION
#define link_foreach(head, itervar) \
    for (struct link_head *itervar = (head)->next; \
            itervar != (head); itervar = itervar->next)

#endif /* end of include guard: LIST_H */
