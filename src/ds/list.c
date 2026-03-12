/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <string.h>
#include "camelot.h"

// --- INTERNAL IMPLEMENTATION ---

static List internal_create(Arena *a, u64 item_size) {
	return (List){
		.source = a,
		.head = NULL,
		.tail = NULL,
		.item_size = item_size,
		.count = 0,
	};
}

static void internal_push(List *l, void *item_ptr) {
	u64 alloc_size = sizeof(struct ListNode) + l->item_size;
	struct ListNode *node = arena.alloc(l->source, alloc_size);

	node->next = NULL;
	node->prev = l->tail;
	memcpy(node->data, item_ptr, l->item_size);

	if (l->tail) {
		l->tail->next = node;
	} else {
		l->head = node;
	}
	l->tail = node;

	l->count++;
}

static void *internal_get(List *l, u64 index) {
	if (index >= l->count)
		return NULL;

	struct ListNode *curr;
	if (index < l->count / 2) {
		curr = l->head;
		for (u64 i = 0; i < index; i++) {
			curr = curr->next;
		}
	} else {
		curr = l->tail;
		for (u64 i = l->count - 1; i > index; i--) {
			curr = curr->prev;
		}
	}

	return curr->data;
}

static void internal_remove(List *l, u64 index) {
	if (index >= l->count)
		return;

	struct ListNode *curr;
	if (index < l->count / 2) {
		curr = l->head;
		for (u64 i = 0; i < index; i++) {
			curr = curr->next;
		}
	} else {
		curr = l->tail;
		for (u64 i = l->count - 1; i > index; i--) {
			curr = curr->prev;
		}
	}

	if (curr->prev) {
		curr->prev->next = curr->next;
	} else {
		l->head = curr->next;
	}

	if (curr->next) {
		curr->next->prev = curr->prev;
	} else {
		l->tail = curr->prev;
	}

	l->count--;
}

// --- NAMESPACE ---

const ListNamespace list = {
	.create = internal_create,
	.push = internal_push,
	.get = internal_get,
	.remove = internal_remove,
};