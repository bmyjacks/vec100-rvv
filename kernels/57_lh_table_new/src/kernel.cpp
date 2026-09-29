/****************************************************************************
 *
 *
 *  Project: json-c 0.19
 *  Source files:
 *    linkhash.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * linkhash.c
 *
 * Copyright (c) 2004, 2005 Metaparadigm Pte. Ltd.
 * Michael Clark <michael@metaparadigm.com>
 * Copyright (c) 2009 Hewlett-Packard Development Company, L.P.
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#include "kernel.h"

#include <assert.h>
#include <stdlib.h>

/*
 * linkhash.c:499-525
 */
struct lh_table *lh_table_new(int size, lh_entry_free_fn *free_fn,
                              lh_hash_fn *hash_fn, lh_equal_fn *equal_fn) {
    int i;
    struct lh_table *t;

    assert(size > 0);
    t = (struct lh_table *)calloc(1, sizeof(struct lh_table));
    if (!t)
        return NULL;

    t->count = 0;
    t->size = size;
    t->table = (struct lh_entry *)calloc(size, sizeof(struct lh_entry));
    if (!t->table) {
        free(t);
        return NULL;
    }
    t->free_fn = free_fn;
    t->hash_fn = hash_fn;
    t->equal_fn = equal_fn;
    for (i = 0; i < size; i++)
        t->table[i].k = LH_EMPTY;
    return t;
}

/*
 * Wrapper for invoking the extracted kernel.
 */
struct lh_table *lh_table_new_isolated(int size, lh_entry_free_fn *free_fn,
                                       lh_hash_fn *hash_fn,
                                       lh_equal_fn *equal_fn) {
    return lh_table_new(size, free_fn, hash_fn, equal_fn);
}
