#include "str.h"

#include <stdlib.h>
#include <string.h>

/* this is a hack to fix a call-graph generation bug with doxygen */
typedef struct String string;
string str_create(const char* ptr, size_t sz) {
	struct String str = {0};
	size_t cap = sz;

	/* auto-size for null-terminated string */
	if (ptr != NULL && sz == 0) {
		sz = strlen(ptr);
	}

	if (ptr != NULL) {
		/* +1 for null terminator */
		cap += 1;
	}

	/* size is the the length of the string, not the buffer */
	if (ptr == NULL) {
		/* no string */
		str.size = 0;
	} else if (sz == 0) {
		/* string is null-terminated */
		str.size = strlen(ptr);
	} else {
		/* string is a sub-string */
		str.size = sz;
	}

	str.data = malloc(cap);
	if (str.data == NULL) {
		/* out of memory, return "null" object */
		str.size = 0;
		str.capacity = 0;
		return str;
	}

	/* avoid unnecessary copying */
	if (ptr != NULL && sz != 0) {
		memcpy(str.data, ptr, sz);
	}
	/* zero rest of buffer, not just null-terminator */
	memset(str.data + sz, 0, cap - sz);

	str.capacity = cap;

	return str;
}

void str_destroy(struct String* str) {
	if (str == NULL) {
		return;
	}

	free(str->data);
	str->data = NULL;
	str->size = 0;
	str->capacity = 0;
}

void str_append(struct String* str, const char* ptr, size_t sz) {
	size_t new_sz = 0;
	char* dst = NULL;

	/* auto-size for null-terminated */
	if (ptr != NULL && sz == 0) {
		sz = strlen(ptr);
	}

	/* check if resize required */
	new_sz = str->size + sz;
	if ((new_sz + 1) > str->capacity) {
		if (!str_reserve(str, new_sz + 1)) {
			/* todo: handle error */
			return;
		}
	}

	/* dst points to the current null-terminator */
	dst = str->data + str->size;
	memcpy(dst, ptr, sz);
	/* new null-terminator must be added */
	str->data[new_sz] = 0;

	str->size = new_sz;
	/* capacity is updated by `str_reserve()` */
}

bool str_reserve(struct String* str, size_t new_sz) {
	char* tmp = realloc(str->data, new_sz);

	if (tmp == NULL) {
		return false;
	}

	str->data = tmp;
	/* size is not changed here */
	str->capacity = new_sz;

	return true;
}
