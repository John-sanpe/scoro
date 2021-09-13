/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _SCORO_LIST_H_
#define _SCORO_LIST_H_

#include <stdbool.h>
#include "scoro_osdep.h"

typedef struct list_head{
    struct list_head *prev, *next;
} list_t;

#define LIST_HEAD(name) \
    struct list_head name = { &(name), &(name) }

static inline void list_head_init(struct list_head *list)
{
    list->prev = list;
    list->next = list;
}

static inline void list_insert(struct list_head *prev,
                 struct list_head *next,
                 struct list_head *new)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

static inline void list_add(struct list_head *head,
              struct list_head *new)
{
    list_insert(head, head->next, new);
}

/**
 * list_add_prev:
 *       add a new node after old node
 * @para node: list head to add it next
 * @para new:  new entry to be added
 *
 * @return None
 */
static inline void list_add_prev(struct list_head *node,
                   struct list_head *new)
{
    list_insert(node->prev , node, new);
}

/**
 * list_add_next:
 *       add a new node next old node
 * @para node: list head to add it next
 * @para new:  new entry to be added
 *
 * @return None
 */
static inline void list_add_next(struct list_head *node,
                   struct list_head *new)
{
    list_insert(node, node->next, new);
}

/**
 * list_del:
 *       in fact, it just connect next and prev node
 * @para node: list head to add it next
 * @para new:  new entry to be added
 *
 * @return None
 */
static inline void list_del(struct list_head *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = NULL;
    node->next = NULL;
}

/*
 * list_replace - Replace a linked list node with an external node
 */
static inline void list_replace(struct list_head *old,
                  struct list_head *new)
{
    new->prev = old->prev;
    new->next = old->next;
    new->prev->next = new;
    new->next->prev = new;
}

/*
 * list_move_tail - Move the node to the head of the list
 */
static inline void list_move_head(struct list_head *head, struct list_head *node)
{
    list_del(node);
    list_add_next(head, node);
}

/*
 * list_move_tail - Move the node to the tail of the list
 */
static inline void list_move_tail(struct list_head *head, struct list_head *node)
{
    list_del(node);
    list_add_prev(head, node);
}

static inline bool list_check_first(struct list_head *head,
                      struct list_head *node)
{
    return node->prev == head;
}

static inline bool list_check_end(struct list_head *head,
                     struct list_head *node)
{
    return node->next == head;
}

#define container_of(ptr, type, member) ({                  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );     \
})

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

/**
 * list_first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

/**
 * list_prev_entry - get the prev element in list
 * @pos:	the type * to cursor
 * @member:	the name of the list_head within the struct.
 */
#define list_prev_entry(pos, member) \
    list_entry((pos)->member.prev, typeof(*(pos)), member)

/**
 * list_next_entry - get the next element in list
 * @pos:	the type * to cursor
 * @member:	the name of the list_head within the struct.
 */
#define list_next_entry(pos, member) \
    list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * list_entry_is_head - test if the entry points to the head of the list
 * @pos:	the type * to cursor
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define list_entry_is_head(pos, head, member) \
    (&pos->member == (head))

/**
 * list_first_entry_or_null - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note that if the list is empty, it returns NULL.
 */
#define list_first_entry_or_null(ptr, type, member) ({          \
    struct list_head *head__ = (ptr);                           \
    struct list_head *pos__ = head__->next;                     \
    pos__ != head__ ? list_entry(pos__, type, member) : NULL;   \
})

/**
 * list_for_each - iterate over a list
 * @head:	the head for your list.
 * @pos:	the &struct list_head to use as a loop cursor.
 */
#define list_for_each(head, pos) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_prev	-	iterate over a list backwards
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define list_for_each_prev(head, pos) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * list_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define list_for_each_entry(pos, head, member)                  \
    for (pos = list_first_entry(head, typeof(*pos), member);    \
         &pos->member != (head);                                \
         pos = list_next_entry(pos, member))

#endif  /* _LIST_H_ */
