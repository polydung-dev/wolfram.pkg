#include <Python.h>

#include <stdbool.h>
#include <stdio.h>

#include "eca.h"
#include "str.h"

/* uint8_t get_mirror_rule(uint8_t r); */
/* ecs.mirror_rule(rule: int, /) -> int */
static PyObject* py_get_mirror_rule(PyObject* self, PyObject* args);

/* uint8_t get_complement_rule(uint8_t r); */
/* ecs.complement_rule(rule: int, /) -> int */
static PyObject* py_get_complement_rule(PyObject* self, PyObject* args);

/* void eca_initialise(uint8_t* dst, size_t width, int channel_count); */
/* ecs.initial_generation(width: int, /, mode: string) -> list */
static PyObject* py_initial_generation(
	PyObject* self, PyObject* args, PyObject* kwargs
);

/* void eca_generate(
	uint8_t* dst, const uint8_t* src, size_t width, int channel_count,
	uint8_t rules[channel_count]
); */
/* ecs.next_generation(prev_gen: list, rule: int, /) -> list */
static PyObject* py_generate(PyObject* self, PyObject* args);

/*****************************************************************************/

static PyMethodDef wolfram_methods[] = {
	{
		"mirror_rule", py_get_mirror_rule, METH_VARARGS,
		"Returns the mirror of the given rule."
	},
	{
		"complement_rule", py_get_complement_rule, METH_VARARGS,
		"Returns the complement of the given rule."
	},
	{
		"initial_generation", (PyCFunction)py_initial_generation,
		METH_VARARGS | METH_KEYWORDS,
		"Returns the initial generation of the automata."
	},
	{
		"next_generation", py_generate, METH_VARARGS,
		"Calculates the next generation of the automata."
	},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef wolfram_module = {
	PyModuleDef_HEAD_INIT,
	"wolfram", "Wolfram Elementary Cellular Automata", -1, wolfram_methods,
	NULL, 0, 0, 0
};

PyObject* PyInit_wolfram(void) {
	return PyModule_Create(&wolfram_module);
}

/*****************************************************************************/

static PyObject* py_get_mirror_rule(PyObject* self, PyObject* args) {
	(void)self;

	const int rule;
	if (!PyArg_ParseTuple(args, "i", &rule)) {
		return NULL;
	}

	return PyLong_FromLong(get_mirror_rule(rule));
}

static PyObject* py_get_complement_rule(PyObject* self, PyObject* args) {
	(void)self;

	const int rule;
	if (!PyArg_ParseTuple(args, "i", &rule)) {
		return NULL;
	}

	return PyLong_FromLong(get_complement_rule(rule));
}

static const int channel_count = 1;
static bool compare(const char* a, const char* b) {
	size_t sz_a = strlen(a);
	size_t sz_b = strlen(b);

	if (sz_a != sz_b) {
		return false;
	}

	return memcmp(a, b, sz_a) == 0;
}

static int kwarg_index(const char* arg, const char* options[], size_t count) {
	for (size_t i = 0; i < count; ++i) {
		if (compare(arg, options[i])) {
			return i;
		}
	}

	return -1;
}

static PyObject* py_initial_generation(
	PyObject* self, PyObject* args, PyObject* kwargs
) {
	(void)self;

	const int width;
	char* mode = "standard";
	static char* kwlist[] = {"", "mode", NULL};

	if (!PyArg_ParseTupleAndKeywords(
		args, kwargs, "i|s", kwlist, &width, &mode
	)) {
		return NULL;
	}

	size_t option_count = 3;

	const char* options[option_count] = {};
	options[0] = "standard";
	options[1] = "alternate";
	options[2] = "random";

	eca_init_fn* functions[option_count] = {};
	functions[0] = eca_initialise;
	functions[1] = eca_initialise_alternate;
	functions[2] = eca_initialise_random;

	eca_init_fn* init_fn = NULL;
	int option_index = kwarg_index(mode, options, option_count);
	if (option_index == -1) {
		struct String buf = str_create(NULL, 64);
		str_append(&buf, "invalid choice: '", 0);
		str_append(&buf, mode, 0);
		str_append(&buf, "' (choose from ", 0);

		for (size_t i = 0; i < option_count; ++i) {
			str_append(&buf, options[i], 0);
			if (i + 1 < option_count) {
				str_append(&buf, ", ", 0);
			}
		}

		str_append(&buf, ")", 0);
		PyErr_SetString(PyExc_TypeError, buf.data);
		str_destroy(&buf);
		return NULL;
	}
	init_fn = functions[option_index];

	uint8_t buffer[width * channel_count] = {};
	init_fn(&buffer[0], width, channel_count);

	PyObject* list = PyList_New(width);
	for (int i = 0; i < width; ++i) {
		PyObject* value = PyLong_FromLong(buffer[i * channel_count]);
		PyList_SetItem(list, i, value);
	}

	return list;
}

static PyObject* py_generate(PyObject* self, PyObject* args) {
	(void)self;

	PyObject* prev = NULL;
	const unsigned int rule;
	if (!PyArg_ParseTuple(args, "Oi", &prev, &rule)) {
		return NULL;
	}

	if (!PyList_Check(prev)) {
		PyErr_SetString(PyExc_TypeError, "not a list");
		return NULL;
	}

	int len = PyList_Size(prev);

	uint8_t src[len * channel_count] = {};
	for (int i = 0; i < len; ++i) {
		uint8_t value = PyLong_AsLong(PyList_GetItem(prev, i));
		for (int j = 0; j < channel_count; ++j)  {
			src[i * channel_count + j] = value;
		}
	}

	uint8_t dst[len * channel_count] = {};
	uint8_t rules[channel_count];
	memset(rules, rule, channel_count);
	eca_generate(dst, src, len, channel_count, rules);

	PyObject* next = PyList_New(len);
	for (int i = 0; i < len; ++i) {
		PyObject* value = PyLong_FromLong(dst[i * channel_count]);
		PyList_SetItem(next, i, value);
	}
	return next;
}
