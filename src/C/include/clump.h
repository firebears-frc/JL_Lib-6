#ifndef CLUMP_H
#define CLUMP_H

#include <stdbool.h>

/** Key comparison.
 */
typedef enum cl_compare_t {
	CL_LESS = -1,		/*< key0 less than key1 */
	CL_EQUAL = 0,		/*< key0 equals key1 */
	CL_GREATER = 1		/*< key0 greater than key1 */
} cl_compare_t;

/** Comparison function callback */
typedef cl_compare_t (cl_compare_cb) (const void *key0, const void *key1);

/** Hash code function callback */
typedef unsigned int (cl_hash_cb) (const void *key);

/* Memory pool functions */
struct cl_pool *cl_pool_create(unsigned int s);
void cl_pool_destroy(struct cl_pool *p);
void *cl_pool_alloc(struct cl_pool *p);
void cl_pool_release(struct cl_pool *p, void *m);
void cl_pool_clear(struct cl_pool *p);

/* Bit array functions */
struct cl_bitarray *cl_bitarray_create(void);
void cl_bitarray_destroy(struct cl_bitarray *ba);
void cl_bitarray_wrap(struct cl_bitarray *ba, unsigned char *buf,
	unsigned int n_bits);
void cl_bitarray_clear(struct cl_bitarray *ba);
unsigned int cl_bitarray_bytes(struct cl_bitarray *ba);
int cl_bitarray_get(struct cl_bitarray *ba, unsigned int i);
int cl_bitarray_pop(struct cl_bitarray *ba);
int cl_bitarray_get_range(struct cl_bitarray *ba, unsigned int i,
	unsigned int n);
int cl_bitarray_pop_range(struct cl_bitarray *ba, unsigned int n);
int cl_bitarray_set(struct cl_bitarray *ba, unsigned int i, unsigned int v);
int cl_bitarray_push(struct cl_bitarray *ba, unsigned int v);
int cl_bitarray_set_range(struct cl_bitarray *ba, unsigned int i,
	unsigned int n, unsigned int v);
int cl_bitarray_push_range(struct cl_bitarray *ba, unsigned int n,
	unsigned int v);

/* Linked list functions */
struct cl_list *cl_list_create(void);
void cl_list_destroy(struct cl_list *list);
bool cl_list_is_empty(struct cl_list *list);
unsigned int cl_list_count(struct cl_list *list);
void *cl_list_add(struct cl_list *list, void *item);
void *cl_list_add_tail(struct cl_list *list, void *item);
void *cl_list_remove(struct cl_list *list, void *item);
bool cl_list_contains(struct cl_list *list, void *item);
void *cl_list_pop(struct cl_list *list);
void cl_list_clear(struct cl_list *list);
struct cl_list_iterator *cl_list_iterator_create(struct cl_list *list);
void cl_list_iterator_destroy(struct cl_list_iterator *it);
void *cl_list_iterator_next(struct cl_list_iterator *it);

/* Hash set/map functions */
struct cl_hash *cl_hash_create_set(cl_hash_cb *hash_func,
	cl_compare_cb *compare);
struct cl_hash *cl_hash_create_map(cl_hash_cb *hash_func,
	cl_compare_cb *compare);
void cl_hash_destroy(struct cl_hash *hash);
unsigned int cl_hash_count(const struct cl_hash *hash);
bool cl_hash_contains(struct cl_hash *hash, const void *key);
const void *cl_hash_peek(struct cl_hash *hash);
const void *cl_hash_get_key(struct cl_hash *hash, const void *key);
const void *cl_hash_get(struct cl_hash *hash, const void *key);
const void *cl_hash_add(struct cl_hash *hash, const void *key);
const void *cl_hash_put(struct cl_hash *hash, const void *key,
	const void *value);
const void *cl_hash_remove(struct cl_hash *hash, const void *key);
void cl_hash_clear(struct cl_hash *hash);
struct cl_hash_iterator *cl_hash_iterator_create(struct cl_hash *hash);
void cl_hash_iterator_destroy(struct cl_hash_iterator *it);
const void *cl_hash_iterator_next(struct cl_hash_iterator *it);
const void *cl_hash_iterator_value(struct cl_hash_iterator *it);
unsigned int cl_hash_str(const void *v);
unsigned int cl_hash_int(const void *v);
cl_compare_t cl_hash_int_compare(const void *v0, const void *v1);
unsigned int cl_hash_ptr(const void *v);
cl_compare_t cl_hash_ptr_compare(const void *v0, const void *v1);

/* Tree set/map functions */
struct cl_tree *cl_tree_create_set(cl_compare_cb *fn_compare);
struct cl_tree *cl_tree_create_map(cl_compare_cb *fn_compare);
void cl_tree_destroy(struct cl_tree *tree);
unsigned int cl_tree_count(struct cl_tree *tree);
bool cl_tree_contains(struct cl_tree *tree, const void *key);
const void *cl_tree_peek(struct cl_tree *tree);
const void *cl_tree_get_key(struct cl_tree *tree, const void *key);
const void *cl_tree_get(struct cl_tree *tree, const void *key);
const void *cl_tree_add(struct cl_tree *tree, const void *key);
const void *cl_tree_put(struct cl_tree *tree, const void *key,
	const void *value);
const void *cl_tree_remove_key(struct cl_tree *tree, const void *key);
const void *cl_tree_remove(struct cl_tree *tree, const void *key);
void cl_tree_clear(struct cl_tree *tree);
struct cl_tree_iterator *cl_tree_iterator_create(struct cl_tree *tree);
void cl_tree_iterator_destroy(struct cl_tree_iterator *it);
const void *cl_tree_iterator_next(struct cl_tree_iterator *it);
const void *cl_tree_iterator_value(struct cl_tree_iterator *it);

/* Huffman codec functions */
struct cl_hcodec *cl_hcodec_create(void);
void cl_hcodec_destroy(struct cl_hcodec *ht);
int cl_hcodec_encode(struct cl_hcodec *ht, const unsigned char *in,
	unsigned int n_in, unsigned char *out, unsigned int n_out);
int cl_hcodec_decode(struct cl_hcodec *ht, unsigned char *in,
	unsigned int n_in, unsigned char *out, unsigned int n_out);

#endif
