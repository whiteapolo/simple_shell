#ifndef DOUBLE_LLL
#define DOUBLE_LLL

#include <stdlib.h>

struct dlist_t;

typedef struct dlist_t {
	struct dlist_t *next;
	struct dlist_t *pre;
	void *info;
} dlist_t;

#define FOR_EACH_NODE(start_node, varname, code) 		\
{ 								\
	dlist_t *_cnt = start_node; 				\
	while (_cnt != NULL) { 					\
		void **varname = &_cnt->info; 			\
		do { code } while (0); 				\
		_cnt = _cnt->next; 				\
	} 							\
}

#define FOR_EACH_NODE_CONDITION(start_node, varname, condition, code) 	\
{ 									\
	dlist_t *_cnt = start_node; 					\
	void **varname = &_cnt->info; 					\
	while (_cnt != NULL && condition) { 				\
		varname = &_cnt->info; 					\
		do { code } while (0); 					\
		_cnt = _cnt->next; 					\
	} 								\
}


void dlist_init(dlist_t **manager);
void dlist_push(dlist_t **manager, void *info);
void *dlist_pop(dlist_t **manager);
void dlist_insert_after(dlist_t *node, void *info);
void dlist_insert_before(dlist_t *node, void *info);
void *dlist_remove(dlist_t **node);
void dlist_clear(dlist_t **manager, void (*free_info)(void *));
void dlist_print(dlist_t *manager, void(*print_info)(void*));

#endif
