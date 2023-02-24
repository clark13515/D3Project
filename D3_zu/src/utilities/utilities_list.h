/*** 
 * @Copyright: Copyright (C) by xxx Ltd
 * @file: 
 * @Brief: 
 * @FilePath: \D3project\D3_zu\src\utilities\utilities_list.h
 * @PROJECT: D3
 * @COMPILER: GCC
 * @HW PLATFORM: ARM
 * @Author: Wang ShiLong
 * @LastEditors: Wang ShiLong
 * @Date: 2023-01-25 10:56:10
 * @LastEditTime: 2023-01-25 11:25:35
 */


#ifndef __UTILITIES_LIST_H__
#define __UTILITIES_LIST_H__

#include <stddef.h>
#include <stdbool.h>
#ifndef __ASSEMBLER__
//=============================================================//
#undef  FORCE_INLINE
#define FORCE_INLINE inline __attribute__((always_inline))

#ifndef offsetof
#define offsetof(type, member)  ((size_t)&((type *)0)->member)
#endif

#ifndef container_of
#define container_of(ptr, type, member)     (type*)((char*)ptr - offsetof(type,member))
#endif
//------------------------------------------------------------//
#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)
struct list_head {
    struct list_head *prev;
    struct list_head *next;
    struct list_head *owner;
};

#define LIST_HEAD_INIT(name)    { &(name), &(name) }
#define LIST_HEAD(name)         struct list_head name = LIST_HEAD_INIT(name)

static FORCE_INLINE void list_head_initialize(struct list_head *list)
{
    list->owner = list;
    list->next = list;
    list->prev = list;
}

static FORCE_INLINE bool list_is_empty(struct list_head *list)
{
    return list->next == list;
}

static FORCE_INLINE bool list_is_single(struct list_head *list)
{
    return (list_is_empty(list)==false) && (list->next == list->prev);
}

static FORCE_INLINE void __list_del(struct list_head *prev, struct list_head *next)
{
    prev->next = next;
    next->prev = prev;
}

static FORCE_INLINE void __list_add(struct list_head *_new, struct list_head *prev, struct list_head *next)
{
    _new->prev = prev;
    _new->next = next;
    prev->next = _new;
    next->prev = _new;
}

static FORCE_INLINE void list_insert(struct list_head *_new, struct list_head *prev, struct list_head *next)
{
    _new->owner = prev->owner;
    __list_add(_new, prev, next);
}

static FORCE_INLINE void list_add_tail(struct list_head *_new, struct list_head *list)
{
    _new->owner = list;
    __list_add(_new, list->prev, list);
}

static FORCE_INLINE void list_add_lead(struct list_head *_new, struct list_head *list)
{
    _new->owner = list;
    __list_add(_new, list, list->next);
}

static FORCE_INLINE void list_del(struct list_head *node)
{
    __list_del(node->prev, node->next);
    node->owner = NULL;
    node->prev = (struct list_head *)LIST_POISON1;
    node->next = (struct list_head *)LIST_POISON2;
}
//---------------------------------------------------------//
#define list_first_entry(list_ptr, type, member)    ((list_ptr)->next != (list_ptr))? container_of((list_ptr)->next, type, member): NULL
#define list_end_entry(list_ptr, type, member)      ((list_ptr)->prev != (list_ptr))? container_of((list_ptr)->prev, type, member): NULL
#define list_prev_entry(node_ptr, type, member)     ((node_ptr)->prev != (node_ptr)->prev->owner) ? container_of((node_ptr)->prev, type, member): NULL
#define list_next_entry(node_ptr, type, member)     ((node_ptr)->next != (node_ptr)->next->owner) ? container_of((node_ptr)->next, type, member): NULL 
//////////////////////
#define list_entry(ptr, type, member)               ((ptr) != (ptr)->owner)? container_of(ptr, type, member) : NULL
#define list_for_each_entry(pos, head, member)  \
        for( pos = list_entry((head)->next, typeof(*pos), member);  \
             pos != NULL; \
             pos = list_entry(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_safe(pos, n, head, member)  \
        for( pos = list_entry((head)->next, typeof(*pos), member), n = (pos)? (list_entry(pos->member.next, typeof(*pos), member)) : NULL; \
             pos != NULL; \
             pos = n, n = ((n!=NULL) ? list_entry(n->member.next, typeof(*pos), member):NULL))
//=============================================================//
#endif    /*__ASSEMBLER__*/
#endif    /*__UTILITIES_LIST_H__*/

