#ifndef STR_H
#define STR_H

#include <stdbool.h>
#include <stddef.h>

/*///////////////////////////////////////////////////////////////////////////*/
/* String                                                                    */
/*///////////////////////////////////////////////////////////////////////////*/

struct String {
	/** @brief a null-terminated string */
	char* data;
	/** @brief strlen(data) */
	size_t size;
	/** @brief size of memory buffer, usually size + 1 */
	size_t capacity;
};

/**
 * @brief Creates a `String` object from a "C string".
 *
 * - If `ptr` == `NULL` and `sz` > `0`, `sz` bytes will be pre-allocated.
 * - If `ptr` != `NULL`, `sz` must be less than or equal to the size of the
 *   buffer given via `ptr`. If `sz` is non-zero, `ptr` is assumed NOT to be
 *   null-terminated so `strlen()` cannot be used to determine buffer length.
 *   This is to allow for sub-strings.
 *
 * @param ptr initial string
 *            - may be `NULL` for empty `String` object
 * @param sz  size of str (not including null terminator)
 *            - if `sz` == `0`, a null-terminated string is assumed
 *
 * @returns	on success	a `String` object with capacity >= 1
 * @returns	on failure	a `String` object with capacity == 0
 */
struct String str_create(const char* ptr, size_t sz);

/**
 * @brief Destroys the `String` object.
 *
 * @param str a pointer to a `String` object
 */
void str_destroy(struct String* str);


/**
 * @brief Append a "C string" to a `String` object.
 *
 * @param str a pointer to a `String` object
 * @param ptr string to append
 * @param sz  size of str (not including null terminator)
 *            - if `sz` == `0`, a null-terminated string is assumed
 */
void str_append(struct String* str, const char* ptr, size_t sz);

/*****************************************************************************/

/**
 * @brief Reserves additional memory.
 *
 * This is called by `str_append()` as needed,
 *
 * @param str    a pointer to a `String` object
 * @param new_sz total size of new buffer (including null-terminator)
 */
bool str_reserve(struct String* str, size_t new_sz);

#endif /* STR_H */
