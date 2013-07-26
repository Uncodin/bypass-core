#! /usr/bin/env bash
# ------------------------------------------------------------------------------
# The MIT License (MIT)
#
# Copyright (c) 2012 Damian Carrillo
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ------------------------------------------------------------------------------
#
# Usage
#
# ./testgen.sh component_test.c component_test_runner.c
#
# Description
#
# In order to minimize the amount of work involved when writing unit tests, this
# script generates test runners for every file supplied as an argument in such
# a way that you, as a developer, only need to define the test cases and any
# hook functions.
#
# This generator lends itself to the xUnit approach to testing, but diverges in
# nature for setup and teardown functions. This script detects hook functions
# with the following names:
#
#	void before_suite() { ... } // Run before a test suite starts
#	void before_test() { ... }	// Run before every test case in the suite
#	void after_test() { ... }	// Run after every test case in the suite
#	void after_suite() { ... }	// Run after the test suite ends
#
# and test functions with names like the following (where the important part is
# the "test" prefix):
#
#	void test_something() { ... }	// Run a test
#	void test_something_else { ... } // Run another test
#
# A test suite is an aggregation of test cases. This script will generate a
# test suite per supplied C module (where a module is passed to this script as
# an argument).
# ------------------------------------------------------------------------------

set -e

function printHeader {
declare module=${1}
declare testSuiteName=${2}

cat <<EOF
#include <stdlib.h>
#include <getopt.h>
#include "sut_test.h"
#include "${module}"

int
main(int argc, char *argv[])
{
	sut_suite_t *suite;
	int ch;
	const char *filter = NULL;

	suite = sut_suite_new("${testSuiteName}");


	// Interpreting arguments

	struct option longopts[] = {
		{ "nofork", no_argument,       NULL, 'n' },
		{ "filter", required_argument, NULL, 'f' },
		{ NULL,     0,                 NULL,  0  }
	};

	while ((ch = getopt_long(argc, (char *const *) argv, "nf:", longopts, NULL)) != -1) {
		switch (ch) {
			case 'n':
				sut_suite_set_fork(suite, false);
				break;
			case 'f':
				filter = optarg;
				break;
			default:
				break;
		}
	}

	argc -= optind;
	argv += optind;

	// Preparing and running tests

EOF
}

function printBeforeTestSuite {
cat <<EOF
	sut_suite_set_before_suite(suite, &before_suite);
EOF
}

function printBeforeTestCase {
cat <<EOF
	sut_suite_set_before_test(suite, &before_test);
EOF
}

function printAfterTestCase {
cat <<EOF
	sut_suite_set_after_test(suite, &after_test);
EOF
}

function printAfterTestSuite {
cat <<EOF
	sut_suite_set_after_suite(suite, &after_suite);
EOF
}

function printTestCase {
declare testCase=${1}

cat <<EOF
	sut_suite_add_test(suite, sut_test_new("${testCase}", &${testCase}));
EOF
}

function printFooter {
cat <<EOF
	sut_suite_run(suite, filter);
	sut_suite_free(suite);

	return 0;
}
EOF
}

declare module=$1
declare outputFile=$2

if [[ -z ${outputFile} ]]; then
	outputFile="${module%%.*}_runner.c"
fi

declare testSuiteName="${module%%.*}"
declare beforeTestSuiteWasFound=
declare beforeTestCaseWasFound=
declare afterTestCaseWasFound=
declare afterTestSuiteWasFound=
declare testCases=

declare functionNames=$(awk '
	/before_suite\((void)?\)/ {
		print "before_suite"
	}

	/before_test\((void)?\)/ {
		print "before_test"
	}

	/after_test\((void)?\)/ {
		print "after_test"
	}

	/after_suite\((void)?\)/ {
		print "after_suite"
	}

	/test.*\((void)?\)/ && !/before_test\((void)?\)/ && !/after_test\((void)?\)/ {
		start = index($0, "test")
		end = index($0, "(") - start
		print substr($0, start, end)
	}
' < ${module})

for functionName in ${functionNames}; do
	if [[ ${functionName} = "before_suite" ]]; then
		beforeTestSuiteWasFound="YES"
	elif [[ ${functionName} = "before_test" ]]; then
		beforeTestCaseWasFound="YES"
	elif [[ ${functionName} = "after_test" ]]; then
		afterTestCaseWasFound="YES"
	elif [[ ${functionName} = "after_suite" ]]; then
		afterTestSuiteWasFound="YES"
	else
		testCases="${testCases} ${functionName}"
	fi
done

printHeader "${module}" "${testSuiteName}" > "${outputFile}"

if [[ ! -z ${beforeTestSuiteWasFound} ]]; then
	printBeforeTestSuite >> "${outputFile}"
fi

if [[ ! -z ${beforeTestCaseWasFound} ]]; then
	printBeforeTestCase >> "${outputFile}"
fi

if [[ ! -z ${afterTestCaseWasFound} ]]; then
	printAfterTestCase >> "${outputFile}"
fi

if [[ ! -z ${afterTestSuiteWasFound} ]]; then
	printAfterTestSuite >> "${outputFile}"
fi

if [[ ! -z ${testCases} ]]; then
	for testCase in ${testCases}; do
		printTestCase "${testCase}" >> "${outputFile}"
	done
fi

printFooter >> "${outputFile}"
