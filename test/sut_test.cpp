// -*- Mode: C; c-file-style: "k&r" -*-
//
// sut_test.h
// Copyright (C) 2012 Damian Carrillo
//
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

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "sut_test.h"

sut_failure_t *_failure;
jmp_buf		_test_env;

////////////////////////////////////////////////////////////////////////////////

void
sut_tap_formatter_before_suite(FILE *out, sut_suite_t *suite)
{
	if (out && suite) {
		fprintf(out, "1..%d\n", suite->count);
		fflush(out);
	}
}

void
sut_tap_formatter_before_test(FILE *out, sut_suite_t *suite, sut_test_t *test)
{
	// do nothing
}

void
sut_tap_formatter_after_test(FILE *out, sut_suite_t *suite, sut_test_t *test)
{
	if (out) {
		bool passed = (test->failure == NULL);
		time_t secs = test->end->tv_sec - test->start->tv_sec;
		suseconds_t usecs = test->end->tv_usec - test->start->tv_usec;

		if (test->error) {
			fprintf(out, "not ok %s (%lds %dµs): %s",
					test->name,
					secs,
					usecs,
					test->error->reason);
		} else {
			fprintf(out, "%s %s (%lds %dµs)",
					passed ? "ok" : "not ok",
					test->name,
					secs,
					usecs);
		}

		if (!passed) {
			fprintf(out, ": assert(%s) in %s line %d",
					test->failure->expression,
					test->failure->file,
					test->failure->line);
		}

		fprintf(out, "\n");
		fflush(out);
	}
}

void
sut_tap_formatter_after_suite(FILE *out, sut_suite_t *suite)
{
	// do nothing
}

sut_formatter_t *
sut_formatter_tap(FILE *out)
{
	sut_formatter_t *tap_formatter = (sut_formatter_t *) malloc(sizeof *tap_formatter);
	tap_formatter->out = out;
	tap_formatter->before_suite = &sut_tap_formatter_before_suite;
	tap_formatter->before_test = &sut_tap_formatter_before_test;
	tap_formatter->after_test = &sut_tap_formatter_after_test;
	tap_formatter->after_suite = &sut_tap_formatter_after_suite;
	return tap_formatter;
}

////////////////////////////////////////////////////////////////////////////////

sut_test_t *
sut_test_new(const char *name, void (*function)(void))
{
	sut_test_t *test = (sut_test_t *) malloc(sizeof *test);
	test->name = name;
	test->function = function;
	test->failure = NULL;
	return test;
}

void
sut_test_free(sut_test_t *test)
{
	if (test) {
		if (test->failure) {
			free(test->failure);
			test->failure = NULL;

			free(test->error);
			test->error = NULL;
		}

		free(test->start);
		free(test->end);

		free(test);
		test = NULL;
	}
}

void
sut_test_run(sut_test_t *test)
{
	if (test) {
		test->function();
	}
}

////////////////////////////////////////////////////////////////////////////////

sut_suite_t *
sut_suite_new(const char *name)
{
	return sut_suite_new_formatted(name, sut_formatter_tap(stdout));
}

sut_suite_t *
sut_suite_new_formatted(const char *name, sut_formatter_t *formatter)
{
	sut_suite_t *suite = (sut_suite_t *) malloc(sizeof *suite);

	if (!name) {
		suite->name = "Unnamed Test Suite";
	} else {
		suite->name = name;
	}

	suite->count = 0;
	suite->formatter = formatter;
	suite->head = NULL;
	suite->fork = true;
	suite->before_suite = NULL;
	suite->before_test = NULL;
	suite->after_test = NULL;
	suite->after_suite = NULL;

	return suite;
}

unsigned int
sut_suite_add_test(sut_suite_t *suite, sut_test_t *test)
{
	if (suite && test) {
		sut_test_elem_t *e = (sut_test_elem_t *) calloc(1, sizeof *e);
		e->test = test;

		if (!suite->head) {
			suite->head = e;
		} else {
			sut_test_elem_t *tail = suite->head;

			while (tail->next) {
				tail = tail->next;
			}

			tail->next = e;
		}

		suite->count++;
	}

	return suite->count;
}

void
sut_suite_set_fork(sut_suite_t *suite, bool fork)
{
	suite->fork = fork;
}

void
sut_suite_set_before_suite(sut_suite_t *suite, void (*before_suite)(void))
{
	suite->before_suite = before_suite;
}

void
sut_suite_set_before_test(sut_suite_t *suite, void (*before_test)(void))
{
	suite->before_test = before_test;
}

void
sut_suite_set_after_test(sut_suite_t *suite, void (*after_test)(void))
{
	suite->after_test = after_test;
}

void
sut_suite_set_after_suite(sut_suite_t *suite, void (*after_suite)(void))
{
	suite->after_suite = after_suite;
}

bool
_sut_run_test(sut_test_t *test)
{
	bool passed;

	_failure = NULL;

	struct timeval *start = (struct timeval *) malloc(sizeof *start);
	struct timeval *end = (struct timeval *) malloc(sizeof *end);

	if (setjmp(_test_env) == 0) {
		gettimeofday(start, NULL);
		sut_test_run(test);
		gettimeofday(end, NULL);

		passed = true;
	} else {
		gettimeofday(end, NULL);
		test->failure = _failure;
		passed = false;
	}

	test->start = start;
	test->end = end;

	return passed;
}

bool
_sut_fork_and_run_test(sut_test_t *test)
{
	bool passed;
	int shmid, status, retval;
	pid_t childpid, endpid;
	sut_fork_result_t *result, *shmresult;

	if ((result = (sut_fork_result_t *) malloc(sizeof *result)) == NULL) {
		fprintf(stderr, "Unable to allocate memory for fork result");
		return false;
	}

	int mode = IPC_CREAT | SHM_R | SHM_W;
	if ((shmid = shmget(IPC_PRIVATE, sizeof(sut_fork_result_t *), mode)) == -1) {
		fprintf(stderr, "Unable to get shared memory segment: %s", strerror(errno));
		return false;
	}

	if ((childpid = fork()) == -1) {   // error
		fprintf(stderr, "Fork error: %s", strerror(errno));
    } else if (childpid == 0) {        // child
		passed = _sut_run_test(test);

		if ((shmresult = (sut_fork_result_t *) shmat(shmid, NULL, 0)) == (void *) -1) {
			fprintf(stderr, "Unable to attach shared memory segment in child: %s",
				strerror(errno));
		}

		if (!passed) {
			retval = 1;
			shmresult->failure = *_failure;
		} else {
			retval = 0;
		}

		shmresult->start = *test->start;
		shmresult->end = *test->end;

		exit(retval);
	} else {                           // parent
		passed = false;

		if ((shmresult = (sut_fork_result_t *) shmat(shmid, NULL, 0)) == (void *) -1) {
			fprintf(stderr, "Unable to attach shared memory segment in parent: %s",
				strerror(errno));
		}

		while (true) {
			endpid = waitpid(childpid, &status, WNOHANG | WUNTRACED);

			if (endpid == -1) {
				fprintf(stderr, "Unable to wait for child process");
				break;
			} else if (endpid == childpid) {
				if (WIFSIGNALED(status) || WIFSTOPPED(status)) {
					test->error = (sut_error_t *) malloc(sizeof test->error);
					test->error->reason = (const char *) malloc(sizeof(char) * 256);

					if (WIFSIGNALED(status)) {
						snprintf((char *) test->error->reason, 255,
							"Child process signaled unexpectedly (%s)",
							strsignal(WTERMSIG(status)));
					} else {
						snprintf((char *) test->error->reason, 255,
							"Child process stopped unexpectedly");
					}
				} else if (WEXITSTATUS(status)) {
					_failure = (sut_failure_t *) malloc(sizeof *_failure);
					_failure->expression = shmresult->failure.expression;
					_failure->file = shmresult->failure.file;
					_failure->line = shmresult->failure.line;

					test->failure = _failure;
				} else {
					passed = true;
				}

				break;
			}
		}

		test->start = (struct timeval *) malloc(sizeof test->start);
		*test->start = shmresult->start;

		test->end = (struct timeval *) malloc(sizeof test->end);
		*test->end = shmresult->end;
	}

	if (shmdt(shmresult) == -1) {
		fprintf(stderr, "Unable to detach shared memory segment\n");
	}

	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		fprintf(stderr, "Unable to remove shared memory segment\n");
	}

	return passed;
}

unsigned int
sut_suite_run(sut_suite_t *suite, const char *filter)
{
	unsigned int failures = 0;

	if (suite) {
		if (suite->formatter) {
			struct timeval suite_start;

			if (gettimeofday(&suite_start, NULL) != -1) {
				suite->start = suite_start;
			}

			if (suite->formatter->before_suite) {
				suite->formatter->before_suite(
					suite->formatter->out,
					suite);
			}
		}

		if (suite->before_suite) {
			suite->before_suite();
		}

		sut_test_elem_t *elem = suite->head;

		while (elem) {
			if (!filter || strcasestr(elem->test->name, filter)) {
				if (suite->formatter) {
					if (suite->formatter->before_test) {
						suite->formatter->before_test(
							suite->formatter->out,
							suite,
							elem->test);
					}
				}

				if (suite->before_test) {
					suite->before_test();
				}

				if (suite->fork) {
					_sut_fork_and_run_test(elem->test);
				} else {
					_sut_run_test(elem->test);
				}

				if (suite->after_test) {
					suite->after_test();
				}

				if (suite->formatter) {
					if (suite->formatter->after_test) {
						const char *expression = NULL;
						const char *file = NULL;
						int line = -1;

						if (elem->test->failure) {
							expression = elem->test->failure->expression;
							file = elem->test->failure->file;
							line = elem->test->failure->line;
						}

						suite->formatter->after_test(
							suite->formatter->out,
							suite,
							elem->test);
					}
				}
			}

			elem = elem->next;
		}

		if (suite->formatter) {
			struct timeval test_end;

			if (gettimeofday(&test_end, NULL) != -1) {
				suite->end = test_end;
			}

			if (suite->formatter->after_suite) {
				suite->formatter->after_suite(
					suite->formatter->out,
					suite);
			}
		}

		if (suite->after_suite) {
			suite->after_suite();
		}
	}

	return failures;
}

void
sut_suite_free(sut_suite_t *suite)
{
	if (suite) {
		if (suite->head) {
			sut_test_elem_t *e = suite->head;
			sut_test_elem_t *f = NULL;

			while (e) {
				f = e;
				e = e->next;

				if (f->test->error && f->test->error->reason) {
					free((char *) f->test->error->reason);
				}

				sut_test_free(f->test);

				free(f);
				f = NULL;
			}

			suite->head = NULL;
		}

		free(suite->formatter);
		suite->formatter = NULL;

		free(suite);
		suite = NULL;
	}
}
