/****************************************************************************
 *
 *
 *  Project: json-c 0.19
 *  Source files:
 *    linkhash.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * linkhash.h
 *
 * Copyright (c) 2004, 2005 Metaparadigm Pte. Ltd.
 * Michael Clark <michael@metaparadigm.com>
 * Copyright (c) 2009 Hewlett-Packard Development Company, L.P.
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#ifndef KERNELS_57_LH_TABLE_NEW_INCLUDE_KERNEL_H_
#define KERNELS_57_LH_TABLE_NEW_INCLUDE_KERNEL_H_

/*
 * linkhash.h:24-26
 */
#ifdef __cplusplus
extern "C" {
#endif

/*
 * linkhash.h:43
 */
#define LH_EMPTY (void *)-1

/*
 * linkhash.h:67-80
 */
struct lh_entry;

typedef void(lh_entry_free_fn)(struct lh_entry *e);
typedef unsigned long(lh_hash_fn)(const void *k);
typedef int(lh_equal_fn)(const void *k1, const void *k2);

/*
 * linkhash.h:85-113
 */
struct lh_entry {
    const void *k;
    int k_is_constant;
    const void *v;
    struct lh_entry *next;
    struct lh_entry *prev;
};

/*
 * linkhash.h:118-162
 */
struct lh_table {
    int size;
    int count;
    struct lh_entry *head;
    struct lh_entry *tail;
    struct lh_entry *table;
    lh_entry_free_fn *free_fn;
    lh_hash_fn *hash_fn;
    lh_equal_fn *equal_fn;
};

/*
 * linkhash.h:198-199
 */
extern struct lh_table *lh_table_new(int size, lh_entry_free_fn *free_fn,
                                     lh_hash_fn *hash_fn,
                                     lh_equal_fn *equal_fn);

/*
 * Wrapper for invoking the extracted kernel.
 */
struct lh_table *lh_table_new_isolated(int size, lh_entry_free_fn *free_fn,
                                       lh_hash_fn *hash_fn,
                                       lh_equal_fn *equal_fn);

/*
 * linkhash.h:455-457
 */
#ifdef __cplusplus
}
#endif

#endif // KERNELS_57_LH_TABLE_NEW_INCLUDE_KERNEL_H_
