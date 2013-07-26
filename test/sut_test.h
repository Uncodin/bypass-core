// -*- Mode: C; c-file-style: "k&r" -*-
//
// sut_test.h
// The MIT License (MIT)
//
// Copyright (c) 2012 Damian Carrillo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#ifndef SUT_TEST_H
#define SUT_TEST_H

// Types ///////////////////////////////////////////////////////////////////////

typedef struct sut_test_t        sut_test_t;
typedef struct sut_suite_t       sut_suite_t;
typedef struct sut_failure_t     sut_failure_t;
typedef struct sut_error_t       sut_error_t;
typedef struct sut_test_elem_t   sut_test_elem_t;
typedef struct sut_fork_result_t sut_fork_result_t;

typedef struct sut_formatter_t {
	FILE *out;
	void (*before_suite)(FILE *out, sut_suite_t *suite);
	void (*before_test) (FILE *out, sut_suite_t *suite, sut_test_t *test);
	void (*after_test)  (FILE *out, sut_suite_t *suite, sut_test_t *test);
	void (*after_suite) (FILE *out, sut_suite_t *suite);
} sut_formatter_t;

struct sut_failure_t {
	const char *expression;
	const char *file;
	int line;
};

struct sut_error_t {
	const char *reason;
	const char *file;
	int line;
};

struct sut_test_t {
	const char *name;
	void (* function)(void);
	sut_failure_t *failure;
	sut_error_t *error;
	struct timeval *start;
	struct timeval *end;
};

struct sut_test_elem_t {
	sut_test_t *test;
	sut_test_elem_t *next;
};

struct sut_fork_result_t {
    sut_failure_t failure;
    struct timeval start;
    struct timeval end;
};

struct sut_suite_t {
	const char *name;
	bool fork;
	const char *filter;
	unsigned int count;
	sut_formatter_t *formatter;
	sut_test_elem_t *head;
	struct timeval start;
	struct timeval end;
	void (*before_suite)(void);
	void (*before_test)(void);
	void (*after_test)(void);
	void (*after_suite)(void);
};

// Variables ///////////////////////////////////////////////////////////////////

extern sut_failure_t *_failure;
extern jmp_buf        _test_env;

// Methods /////////////////////////////////////////////////////////////////////

#undef sut_assert
#define sut_assert(expr)                                      \
	if (!(expr)) {                                            \
		_failure = (sut_failure_t *)malloc(sizeof *_failure); \
		_failure->expression = (const char *) #expr;          \
		_failure->file = (const char *) __FILE__;             \
		_failure->line = (int) __LINE__;                      \
		longjmp(_test_env, 1);                                \
	}

extern sut_formatter_t *sut_formatter_tap(FILE *out);

extern sut_test_t      *sut_test_new(const char *name, void (*test)(void));
extern void             sut_test_free(sut_test_t *test);

extern sut_suite_t     *sut_suite_new(const char *name);
extern sut_suite_t     *sut_suite_new_formatted(const char *name, sut_formatter_t *formatter);
extern unsigned int     sut_suite_add_test(sut_suite_t *suite, sut_test_t *test);
extern void             sut_suite_set_fork(sut_suite_t *suite, bool fork);
extern void             sut_suite_set_before_suite(sut_suite_t *suite, void (*before_suite)(void));
extern void             sut_suite_set_before_test(sut_suite_t *suite, void (*before_test)(void));
extern void             sut_suite_set_after_test(sut_suite_t *suite, void (*after_test)(void));
extern void             sut_suite_set_after_suite(sut_suite_t *suite, void (*after_suite)(void));
extern unsigned int     sut_suite_run(sut_suite_t *suite, const char *filter);
extern void             sut_suite_free(sut_suite_t *suite);

#endif /* SUT_TEST_H */
