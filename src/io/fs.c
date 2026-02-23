/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

// clang-format off
#include <stdio.h>
#include "camelot.h"
// clang-format on

// --- EXTERNAL LINKAGE ---
extern String scan(Arena *a, u64 cap);
extern void put(String s);
extern void print(const char *fmt, ...);

// --- INTERNAL IMPLEMENTATION ---

static u64 internal_stream(File *f, Op op, void *arg, u64 num) {
	if (!f) {
		return 0;
	}

	switch (op) {
	case OPEN: {
		const char *path = (const char *)arg;
		FILE *h = fopen(path, "rb");
		if (!h) {
			f->status = FILE_NOT_FOUND;
			f->size = 0;
			return 0;
		}

		if (fseek(h, 0, SEEK_END) == 0) {
			long len = ftell(h);
			fseek(h, 0, SEEK_SET);
			f->size = (u64)len;
		} else {
			clearerr(h);
			f->size = 0;
		}
		f->handle = h;
		f->status = OK;
		return 1;
	}

	case READ: {
		if (f->status != OK || !f->handle) {
			return 0;
		}
		return fread(arg, 1, num, (FILE *)f->handle);
	}

	case SKIP: {
		if (f->status != OK || !f->handle) {
			return 0;
		}
		fseek((FILE *)f->handle, (long)num, SEEK_CUR);
		return 0;
	}

	case CLOSE: {
		if (f->handle) {
			fclose((FILE *)f->handle);
			f->handle = NULL;
			f->status = IO_ERROR;
		}
		return 0;
	}
	}
	return 0;
}

static String internal_slurp(Arena *a, const char *path) {
	File f = {0};
	if (!internal_stream(&f, OPEN, (void *)path, 0)) {
		return (String){0};
	}

	if (f.size == 0) {
		internal_stream(&f, CLOSE, NULL, 0);
		return (String){0};
	}

	u8 *buf = arena.alloc(a, f.size + 1);
	if (!buf) {
		internal_stream(&f, CLOSE, NULL, 0);
		return (String){0};
	}

	internal_stream(&f, READ, buf, f.size);
	buf[f.size] = '\0';

	internal_stream(&f, CLOSE, NULL, 0);
	return (String){.ptr = buf, .len = f.size};
}

// --- NAMESPACE ---

const IONamespace io = {
	.scan = scan,
	.put = put,
	.print = print,
	.stream = internal_stream,
	.slurp = internal_slurp,
};