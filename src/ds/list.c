/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

// clang-format off
#include <string.h>
#include "camelot.h"
// clang-format on

// --- INTERNAL IMPLEMENTATION ---

static List internal_create(Arena *a, u64 item_size) {
	u64 initial_cap = 16;
	void **dir = arena.alloc(a, sizeof(void *) * initial_cap);

	return (List){
		.source = a,
		.pages = dir,
		.pages_cap = initial_cap,
		.pages_len = 0,
		.item_size = item_size,
		.count = 0,
	};
}

static void ensure_directory(List *l) {
	if (l->pages_len < l->pages_cap)
		return;

	u64 new_cap = l->pages_cap * 2;
	void **new_dir = arena.alloc(l->source, sizeof(void *) * new_cap);

	if (l->pages) {
		memcpy(new_dir, l->pages, sizeof(void *) * l->pages_cap);
	}
	l->pages = new_dir;
	l->pages_cap = new_cap;
}

static void internal_push(List *l, void *item_ptr) {
	u64 page_idx = l->count / PAGE_SIZE;
	u64 item_idx = l->count % PAGE_SIZE;

	if (item_idx == 0) {
		ensure_directory(l);
		void *new_page = arena.alloc(l->source, l->item_size * PAGE_SIZE);
		l->pages[page_idx] = new_page;
		l->pages_len++;
	}

	u8 *target = (u8 *)l->pages[page_idx] + (item_idx * l->item_size);
	memcpy(target, item_ptr, l->item_size);
	l->count++;
}

static void *internal_get(List *l, u64 index) {
	if (index >= l->count)
		return NULL;
	u64 page_idx = index / PAGE_SIZE;
	u64 item_idx = index % PAGE_SIZE;
	return (u8 *)l->pages[page_idx] + (item_idx * l->item_size);
}

static void internal_remove(List *l, u64 index) {
	if (index >= l->count)
		return;

	void *victim = internal_get(l, index);
	void *last = internal_get(l, l->count - 1);

	if (index != l->count - 1) {
		memcpy(victim, last, l->item_size);
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