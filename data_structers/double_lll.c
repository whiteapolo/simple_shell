#include "double_lll.h"
#include <stdlib.h>

void dlist_init(dlist_t **manager)
{
	*manager = NULL;
}

void dlist_push(dlist_t **manager, void *info)
{
	dlist_t *n = malloc(sizeof(dlist_t));
	n->info = info;
	n->pre = NULL;

	n->next = *manager;

	if (*manager)
		(*manager)->pre = n;

	*manager = n;
}

void *dlist_pop(dlist_t **manager)
{
	void *info = (*manager)->info;
	dlist_t *tmp = *manager;

	if ((*manager)->next) {
		*manager = (*manager)->next;
		(*manager)->pre = NULL;
	} else {
		*manager = NULL;
	}
	free(tmp);
	return info;
}

void dlist_insert_after(dlist_t *node, void *info)
{
	dlist_t *n = malloc(sizeof(dlist_t));
	n->info = info;
	n->pre = node;

	n->next = node->next;
	if (node->next != NULL)
		node->next->pre = n;
	node->next = n;
}

void dlist_insert_before(dlist_t *node, void *info)
{
	dlist_t *n = malloc(sizeof(dlist_t));
	n->info = info;

	n->next = node;
	n->pre = node->pre;

	node->pre->next = n;
	node->pre = n;
}

void *dlist_remove(dlist_t **node)
{
	void *info = (*node)->info;
	dlist_t *new = (*node)->pre;

	(*node)->pre->next = (*node)->next;

	if ((*node)->next)
		(*node)->next->pre = (*node)->pre;

	free(*node);
	*node = new;

	return info;
}

void dlist_clear(dlist_t **manager, void (*free_info)(void *))
{
	while (*manager) {
		void *tmp = dlist_pop(manager);
		if (free_info)
			free_info(tmp);
		
	}
	/* void *tmp = dlist_remove(manager, *manager); */
	/* while (*manager != NULL) { */
	/* 	free(tmp); */
	/* 	tmp = dlist_remove(manager, *manager); */
	/* 	*manager = (*manager)->next; */
	/* } */
	/* free(tmp); */
}

void dlist_print(dlist_t *manager, void(*print_info)(void*))
{
	while (manager) {
		print_info(manager->info);
		manager = manager->next;
	}
}
