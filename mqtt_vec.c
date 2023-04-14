//
// Created by zr on 23-4-14.
//
#include "mqtt_vec.h"
#include <stdlib.h>
#include <string.h>

tmq_vec_base_t* tmq_base_init_(size_t elem_size)
{
    tmq_vec_base_t* base = (tmq_vec_base_t*)malloc(sizeof(tmq_vec_base_t));
    if(!base)
        return NULL;
    memset(base, 0, sizeof(tmq_vec_base_t));
    base->elem_size = elem_size;
    return base;
}

static void* index_to_addr(tmq_vec_base_t* v, size_t index)
{
    return (char*)v->data + index * v->elem_size;
}

static int tmq_vec_grow(tmq_vec_base_t* v)
{
    size_t cap = v->cap < 8 ? 8 : v->cap * 2;
    void* data = realloc(v->data, cap * v->elem_size);
    if(!data)
        return -1;
    v->data = data;
    v->cap = cap;
    return 0;
}

int tmq_vec_push_back_(tmq_vec_base_t* v, const void* elem)
{
    if(!v)
        return -1;
    if(v->size >= v->cap)
    {
        if(tmq_vec_grow(v) < 0)
            return -1;
    }
    memcpy(index_to_addr(v, v->size), elem, v->elem_size);
    v->size++;
    return 0;
}

int tmq_vec_insert_(tmq_vec_base_t* v, int index, const void* elem)
{
    if(!v || index < 0 || index > v->size)
        return -1;
    if(v->size >= v->cap)
    {
        if(tmq_vec_grow(v) < 0)
            return -1;
    }
    memmove(index_to_addr(v, index + 1), index_to_addr(v, index), (v->size - index) * v->elem_size);
    memcpy(index_to_addr(v, index), elem, v->elem_size);
    v->size++;
    return 0;
}

int tmq_vec_erase_(tmq_vec_base_t* v, int index)
{
    if(!v || index < 0 || index >= v->size)
        return -1;
    memmove(index_to_addr(v, index), index_to_addr(v, index + 1), (v->size - index - 1) * v->elem_size);
    v->size--;
}

void* tmq_vec_get_(tmq_vec_base_t* v, int index)
{
    if(!v || index < 0 || index >= v->size)
        return NULL;
    return index_to_addr(v, index);
}

void* tmq_vec_begin_(tmq_vec_base_t* v)
{
    if(!v)
        return NULL;
    return v->data;
}

void* tmq_vec_end_(tmq_vec_base_t* v)
{
    if(!v)
        return NULL;
    return index_to_addr(v, v->size);
}

void tmq_vec_clear_(tmq_vec_base_t* v) {v->size = 0;}

void tmq_vec_free_(tmq_vec_base_t* v)
{
    if(!v) return;
    free(v);
}

size_t tmq_vec_size_(tmq_vec_base_t* v)
{
    if(!v) return 0;
    return v->size;
}

int tmq_vec_empty_(tmq_vec_base_t* v)
{
    if(!v) return 1;
    return v->size == 0;
}