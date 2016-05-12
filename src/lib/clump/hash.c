/*
 * hash.c	A generic hash-set or -map
 *
 * Copyright (c) 2007-2012  Douglas P Lau
 *
 * Public functions:
 *
 *	cl_hash_create_set	Create a hash set
 *	cl_hash_create_map	Create a hash map
 *	cl_hash_destroy		Destroy a hash set or map
 *	cl_hash_count		Count the entries in a hash set or map
 *	cl_hash_contains	Test if a hash contains a key
 *	cl_hash_peek		Get an arbitrary key
 *	cl_hash_get_key		Get a key from a hash set
 *	cl_hash_get		Get a value from a hash map
 *	cl_hash_add		Add an entry to a hash set
 *	cl_hash_put		Put a mapping into a hash map
 *	cl_hash_remove		Remove a key from a hash set or map
 *	cl_hash_clear		Clear all entries from a hash set or map
 *	cl_hash_iterator_create Create a hash key iterator
 *	cl_hash_iterator_destroy Destroy a hash key iterator
 *	cl_hash_iterator_next	Get the next key from an iterator
 *	cl_hash_iterator_value	Get the value mapped to most recent key
 *	cl_hash_str		Hash function for strings
 *	cl_hash_int		Hash function for ints
 *	cl_hash_ptr		Hash function for pointers
 */
/** \file
 *
 * The hash module provides both hash sets and hash maps.
 * A hash map uses a bit more memory than a hash set, but allows an arbitrary
 * value to be mapped to each hash key.
 * Users must provide a hash function, which calculates a stable hash code
 * for keys.
 * NOTE: some functions can be used with either hash sets or maps, but some
 * must only be used with either sets or maps.
 */
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "clump.h"

/** Hash set entry structure.
 */
struct cl_hash_entry {
	struct cl_hash_entry	*next;	/*< link to next hash entry */
	const void		*key;	/*< key stored in hash entry */
};

/** Hash mapping structure.
 */
struct cl_hash_mapping {
	struct cl_hash_entry	entry;	/*< hash entry struct */
	const void		*value;	/*< value for hash mapping */
};

/** Hash iterator structure.
 */
struct cl_hash_iterator {
	struct cl_hash		*hash;		/**< hash struct */
	struct cl_hash_entry	*curr;		/**< current entry in bucket */
	unsigned int		bucket;		/**< current bucket */
};

/** Hash table structure.
 */
struct cl_hash {
	cl_hash_cb		*fn_hash;	/*< hash function */
	cl_compare_cb		*fn_compare;	/*< comparision function */
	struct cl_pool		*pool;		/*< hash entry / mapping pool */
	void			**table;	/*< actual hash table */
	unsigned int		n_prime;	/*< index into prime array */
	unsigned int		n_entries;	/*< number of entries */
};

/* Prime numbers used for hash table sizes */
static const unsigned int CL_HASH_PRIMES[] = {
	53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
	196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
	50331653, 100663319, 201326611, 402653189, 805306457, 1610612741
};

/* Number of primes defined for hash table sizes */
static const unsigned int CL_HASH_N_PRIMES =
	sizeof(CL_HASH_PRIMES) / sizeof(unsigned int);

/** Get the number of buckets.
 *
 * @param hash Pointer to hash table.
 * @return Current number of buckets in the hash table.
 */
static inline unsigned int cl_hash_buckets(const struct cl_hash *hash) {
	return CL_HASH_PRIMES[hash->n_prime];
}

/** Get the bucket for a hash value.
 *
 * @param hash Pointer to hash table.
 * @param hcode Hash code.
 * @return Bucket in the hash table for the specified hash value.
 */
static inline unsigned int cl_hash_bucket(const struct cl_hash *hash,
	unsigned int hcode)
{
	return hcode % cl_hash_buckets(hash);
}

/** Get the hash entry minimum limit.
 *
 * @param hash Pointer to hash table.
 * @return Current hash entry minimum limit.
 */
static inline unsigned int cl_hash_slimit(const struct cl_hash *hash) {
	unsigned int n_prime = hash->n_prime;
	if(n_prime)
		return CL_HASH_PRIMES[n_prime - 1] / 2;
	else
		return 0;
}

/** Get the hash entry limit.
 *
 * A hash table should never be more than 2/3 full, so the limit should be
 * checked before adding a new entry.
 *
 * @param hash Pointer to hash table.
 * @return Current hash entry limit.
 */
static inline unsigned int cl_hash_limit(const struct cl_hash *hash) {
	return cl_hash_buckets(hash) * 2 / 3;
}

/** Allocate hash table.
 *
 * Allocate memory for hash table buckets.
 */
static inline void cl_hash_table_alloc(struct cl_hash *hash) {
	unsigned int n_buckets = cl_hash_buckets(hash);

	hash->table = malloc(sizeof(void *) * n_buckets);
	assert(hash->table);
	memset(hash->table, 0, sizeof(void *) * n_buckets);
}

/** Create a hash set or map.
 *
 * Create a hash set or map, preparing it to be used.
 *
 * @param fn_hash Function to calculate a hash code.
 * @param fn_compare Function to compare two keys for equality.
 * @return Pointer to hash set.
 */
static struct cl_hash *cl_hash_create(cl_hash_cb *fn_hash,
	cl_compare_cb *fn_compare)
{
	struct cl_hash *hash = malloc(sizeof(struct cl_hash));

	assert(hash);
	hash->pool = NULL;
	hash->n_prime = 0;
	hash->n_entries = 0;
	hash->fn_hash = fn_hash;
	hash->fn_compare = fn_compare;
	cl_hash_table_alloc(hash);
	return hash;
}

/** Create a hash set.
 *
 * Create a hash set, preparing it to be used.
 *
 * @param fn_hash Function to calculate a hash code.
 * @param fn_compare Function to compare two keys for equality.
 * @return Pointer to hash set.
 */
struct cl_hash *cl_hash_create_set(cl_hash_cb *fn_hash,
	cl_compare_cb *fn_compare)
{
	struct cl_hash *hash = cl_hash_create(fn_hash, fn_compare);
	hash->pool = cl_pool_create(sizeof(struct cl_hash_entry));
	return hash;
}

/** Create a hash map.
 *
 * Create a hash map, preparing it to be used.
 *
 * @param fn_hash Function to calculate a hash code.
 * @param fn_compare Function to compare two keys for equality.
 * @return Pointer to hash map.
 */
struct cl_hash *cl_hash_create_map(cl_hash_cb *fn_hash,
	cl_compare_cb *fn_compare)
{
	struct cl_hash *hash = cl_hash_create(fn_hash, fn_compare);
	hash->pool = cl_pool_create(sizeof(struct cl_hash_mapping));
	return hash;
}

/** Destroy a hash table.
 *
 * Destroy a hash table, freeing all its resources.
 *
 * @param hash Pointer to hash table.
 */
void cl_hash_destroy(struct cl_hash *hash) {
	cl_pool_destroy(hash->pool);
	free(hash->table);
	hash->table = NULL;
	free(hash);
}

/** Get the count of entries.
 *
 * @param hash Pointer to hash set or map.
 * @return Count of entries currently in the hash set or map.
 */
unsigned int cl_hash_count(const struct cl_hash *hash) {
	return hash->n_entries;
}

/** Test if a hash entry equals a key.
 *
 * @param hash Pointer to hash set or map.
 * @param e Hash entry.
 * @param key Key to compare.
 * @param hcode Hash code.
 * @return True if they are equal.
 */
static inline bool cl_hash_equals(const struct cl_hash *hash,
	const struct cl_hash_entry *e, const void *key, unsigned int hcode)
{
	unsigned int hc = hash->fn_hash(e->key);
	return (hcode == hc) && (hash->fn_compare(key, e->key) == CL_EQUAL);
}

/** Test if a hash contains a key.
 *
 * Test if a hash (set or map) contains the specified key.
 *
 * @param hash Pointer to hash table.
 * @param key Key to test for.
 * @return True if hash contains the key, otherwise false.
 */
bool cl_hash_contains(struct cl_hash *hash, const void *key) {
	unsigned int hcode = hash->fn_hash(key);
	unsigned int bucket = cl_hash_bucket(hash, hcode);
	struct cl_hash_entry *e = hash->table[bucket];
	while(e) {
		if(cl_hash_equals(hash, e, key, hcode))
			return true;
		e = e->next;
	}
	return false;
}

/** Get an arbitrary key.
 *
 * Get an arbitrary key from a hash set or map.
 *
 * @param hash Pointer to hash set or map.
 * @return Pointer to key, or NULL if hash is empty.
 */
const void *cl_hash_peek(struct cl_hash *hash) {
	unsigned int i;
	for(i = 0; i < cl_hash_buckets(hash); i++) {
		struct cl_hash_entry *e = hash->table[i];
		if(e)
			return e->key;
	}
	return NULL;
}

/** Get a key from a hash set.
 *
 * @deprecated
 *
 * @param hash Pointer to hash set.
 * @param key Key to look up.
 * @return Matching key from hash set.
 */
const void *cl_hash_get_key(struct cl_hash *hash, const void *key) {
	unsigned int hcode = hash->fn_hash(key);
	unsigned int bucket = cl_hash_bucket(hash, hcode);
	struct cl_hash_entry *e = hash->table[bucket];
	while(e) {
		if(cl_hash_equals(hash, e, key, hcode))
			return e->key;
		e = e->next;
	}
	return NULL;
}

/** Get a value from a hash map.
 *
 * Get a value assocated with the given key from a hash map.  NOTE: do not use
 * this function for hash sets; use cl_hash_contains instead.
 *
 * @param hash Pointer to hash map.
 * @param key Key to look up.
 * @return value Associated with key, or NULL if not found.
 */
const void *cl_hash_get(struct cl_hash *hash, const void *key) {
	unsigned int hcode = hash->fn_hash(key);
	unsigned int bucket = cl_hash_bucket(hash, hcode);
	struct cl_hash_entry *e = hash->table[bucket];
	while(e) {
		if(cl_hash_equals(hash, e, key, hcode)) {
			struct cl_hash_mapping *m = (struct cl_hash_mapping *)e;
			return m->value;
		}
		e = e->next;
	}
	return NULL;
}

/** Resize a hash table.
 *
 * Resize a hash table by copying all the hash entries into a new table.
 *
 * @param hash Pointer to hash table.
 * @param n_prime Index into prime number array.
 */
static void cl_hash_resize(struct cl_hash *hash, unsigned int n_prime) {
	const unsigned int o_buckets = cl_hash_buckets(hash);
	void **o_table = hash->table;
	unsigned int i;

	hash->n_prime = n_prime;
	cl_hash_table_alloc(hash);

	for(i = 0; i < o_buckets; i++) {
		struct cl_hash_entry *e = o_table[i];
		while(e) {
			struct cl_hash_entry *next = e->next;
			unsigned int hcode = hash->fn_hash(e->key);
			unsigned int bucket = cl_hash_bucket(hash, hcode);
			e->next = hash->table[bucket];
			hash->table[bucket] = e;
			e = next;
		}
	}
	free(o_table);
}

/** Expand a hash table.
 *
 * Expand a hash table by copying all the hash entries into a new, larger table.
 *
 * @param hash Pointer to hash table.
 */
static void cl_hash_expand(struct cl_hash *hash) {
	if(hash->n_prime + 1 < CL_HASH_N_PRIMES)
		cl_hash_resize(hash, hash->n_prime + 1);
}

/** Add a new entry to a hash set.
 *
 * Add a new entry into a hash set.  The hash table will be expanded if
 * necessary.  NOTE: do not use this function for hash maps; use cl_hash_put
 * instead.
 *
 * @param hash Pointer to hash set.
 * @param key Key to add to hash set.
 * @return Key added to hash set.
 */
const void *cl_hash_add(struct cl_hash *hash, const void *key) {
	unsigned int hcode = hash->fn_hash(key);
	unsigned int bucket = cl_hash_bucket(hash, hcode);
	struct cl_hash_entry *e = cl_pool_alloc(hash->pool);

	assert(e);
	if(hash->n_entries >= cl_hash_limit(hash))
		cl_hash_expand(hash);
	hash->n_entries++;
	e->key = key;
	e->next = hash->table[bucket];
	hash->table[bucket] = e;
	return key;
}

/** Add a new mapping to a hash map.
 *
 * Add a new mapping into a hash map.  The hash table will be expanded if
 * necessary.  NOTE: do not use this function for hash sets; use cl_hash_add
 * instead.
 *
 * @param hash Pointer to hash map.
 * @param key Key to put into hash map.
 * @param value Value to associate with key.
 * @return Previous value associated with key.
 */
const void *cl_hash_put(struct cl_hash *hash, const void *key,
	const void *value)
{
	unsigned int hcode = hash->fn_hash(key);
	unsigned int bucket = cl_hash_bucket(hash, hcode);
	struct cl_hash_mapping *m = cl_pool_alloc(hash->pool);
	struct cl_hash_entry *e = &m->entry;

	assert(m);
	if(hash->n_entries >= cl_hash_limit(hash))
		cl_hash_expand(hash);
	hash->n_entries++;
	e->next = hash->table[bucket];
	e->key = key;
	m->value = value;
	hash->table[bucket] = e;
	return key;
}

/** Shrink a hash table.
 *
 * Shrink a hash table by copying all the hash entries into a new, smaller
 * table.
 *
 * @param hash Pointer to hash table.
 */
static void cl_hash_shrink(struct cl_hash *hash) {
	if(hash->n_prime > 0)
		cl_hash_resize(hash, hash->n_prime - 1);
}

/** Remove an entry from a hash set or map.
 *
 * Remove the specified entry from a hash set or map.
 *
 * @param hash Pointer to hash table.
 * @param key Key to be removed.
 * @return Key removed, or NULL if not found.
 */
const void *cl_hash_remove(struct cl_hash *hash, const void *key) {
	struct cl_hash_entry *prev = NULL;
	unsigned int hcode = hash->fn_hash(key);
	unsigned int bucket = cl_hash_bucket(hash, hcode);
	struct cl_hash_entry *e = hash->table[bucket];
	while(e) {
		if(cl_hash_equals(hash, e, key, hcode)) {
			key = e->key;
			if(prev)
				prev->next = e->next;
			else
				hash->table[bucket] = e->next;
			hash->n_entries--;
			cl_pool_release(hash->pool, e);
			if(hash->n_entries == cl_hash_slimit(hash))
				cl_hash_shrink(hash);
			return key;
		}
		prev = e;
		e = e->next;
	}
	return NULL;
}

/** Clear a hash set or map.
 *
 * Remove all entries from a hash set or map.
 *
 * @param hash Pointer to hash set or map.
 */
void cl_hash_clear(struct cl_hash *hash) {
	assert(hash);
	if(hash->n_prime) {
		free(hash->table);
		hash->n_prime = 0;
		cl_hash_table_alloc(hash);
	} else
		memset(hash->table, 0, sizeof(void *) * cl_hash_buckets(hash));
	cl_pool_clear(hash->pool);
	hash->n_entries = 0;
}

/** Create a hash iterator.
 *
 * @param hash Pointer to hash set or map.
 * @return Iterator for hash keys.
 */
struct cl_hash_iterator *cl_hash_iterator_create(struct cl_hash *hash) {
	struct cl_hash_iterator *it = malloc(sizeof(struct cl_hash_iterator));
	assert(hash);
	it->hash = hash;
	it->curr = NULL;
	it->bucket = 0;
	return it;
}

/** Destroy a hash iterator.
 *
 * @param it Hash key iterator.
 */
void cl_hash_iterator_destroy(struct cl_hash_iterator *it) {
	/* Make sure user doesn't reuse iterator after destroying */
	it->hash = NULL;
	it->curr = NULL;
	free(it);
}

/** Get the next key from a hash iterator.
 *
 * @return Next key in hash iterator, or NULL.
 */
const void *cl_hash_iterator_next(struct cl_hash_iterator *it) {
	struct cl_hash *hash = it->hash;
	struct cl_hash_entry *e = it->curr;
	assert(hash);
	if(e)
		e = e->next;
	while(e == NULL && it->bucket < cl_hash_buckets(hash))
		e = hash->table[it->bucket++];
	if(e) {
		it->curr = e;
		return e->key;
	} else {
		it->curr = NULL;
		it->bucket = 0;
		return NULL;
	}
}

/** Get the value associated with most recent key from a hash iterator.
 *
 * @param it The iterator.
 * @return Value associated with most recent key returned, or NULL.
 */
const void *cl_hash_iterator_value(struct cl_hash_iterator *it) {
	struct cl_hash_mapping *m = (struct cl_hash_mapping *)it->curr;
	if(m)
		return m->value;
	else
		return NULL;
}

/** String hash function.
 *
 * Simple hash function for a C string.
 */
unsigned int cl_hash_str(const void *v) {
	const unsigned char *str = v;
	unsigned int hash = 5381;
	int c;
	while((c = *str)) {
		hash = ((hash << 5) + hash) + c;
		str++;
	}
	return hash;
}

/** Int hash function.
 *
 * Simple hash function for an int.
 */
unsigned int cl_hash_int(const void *v) {
	int i = (int)(long)v;
	return i;
}

/** Int hash compare function.
 */
cl_compare_t cl_hash_int_compare(const void *v0, const void *v1) {
	int i0 = (int)(long)v0;
	int i1 = (int)(long)v1;
	if(i0 > i1)
		return CL_GREATER;
	if(i0 < i1)
		return CL_LESS;
	return CL_EQUAL;
}

/** Pointer hash function.
 *
 * Simple hash function for a pointer.
 */
unsigned int cl_hash_ptr(const void *v) {
	long p = (long)v;
	return (p >> (4 * sizeof(long))) ^ p;
}

/** Pointer hash compare function.
 */
cl_compare_t cl_hash_ptr_compare(const void *v0, const void *v1) {
	if(v0 > v1)
		return CL_GREATER;
	if(v0 < v1)
		return CL_LESS;
	return CL_EQUAL;
}
