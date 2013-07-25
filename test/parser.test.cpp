#include <stdlib.h>
#include <getopt.h>
#include "sut_test.h"
#include "parser.test.tpp"

int
main(int argc, char *argv[])
{
	sut_suite_t *suite;
	int ch;
	const char *filter = NULL;

	suite = sut_suite_new("parser");


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

	sut_suite_add_test(suite, sut_test_new("test_parse_null", &test_parse_null));
	sut_suite_add_test(suite, sut_test_new("test_parse_empty", &test_parse_empty));
	sut_suite_add_test(suite, sut_test_new("test_parse_text", &test_parse_text));
	sut_suite_add_test(suite, sut_test_new("test_parse_emphasis_with_simple_example", &test_parse_emphasis_with_simple_example));
	sut_suite_add_test(suite, sut_test_new("test_parse_single_interspersed_emphasis", &test_parse_single_interspersed_emphasis));
	sut_suite_add_test(suite, sut_test_new("test_parse_multiple_interspersed_emphasis", &test_parse_multiple_interspersed_emphasis));
	sut_suite_add_test(suite, sut_test_new("test_parse_double_emphasis_with_simple_example", &test_parse_double_emphasis_with_simple_example));
	sut_suite_add_test(suite, sut_test_new("test_parse_single_interspersed_double_emphasis", &test_parse_single_interspersed_double_emphasis));
	sut_suite_add_test(suite, sut_test_new("test_parse_multiple_interspersed_double_emphasis", &test_parse_multiple_interspersed_double_emphasis));
	sut_suite_add_test(suite, sut_test_new("test_parse_triple_emphasis_with_simple_example", &test_parse_triple_emphasis_with_simple_example));
	sut_suite_add_test(suite, sut_test_new("test_parse_single_interspersed_triple_emphasis", &test_parse_single_interspersed_triple_emphasis));
	sut_suite_add_test(suite, sut_test_new("test_parse_multiple_interspersed_triple_emphasis", &test_parse_multiple_interspersed_triple_emphasis));
	sut_suite_add_test(suite, sut_test_new("test_parse_strikethrough_with_simple_example", &test_parse_strikethrough_with_simple_example));
	sut_suite_add_test(suite, sut_test_new("test_parse_single_interspersed_strikethrough", &test_parse_single_interspersed_strikethrough));
	sut_suite_add_test(suite, sut_test_new("test_parse_multiple_interspersed_strikethrough", &test_parse_multiple_interspersed_strikethrough));
	sut_suite_add_test(suite, sut_test_new("test_parse_link_with_simple_example", &test_parse_link_with_simple_example));
	sut_suite_add_test(suite, sut_test_new("test_parse_link_with_simple_titled_example", &test_parse_link_with_simple_titled_example));
	sut_suite_add_test(suite, sut_test_new("test_parse_link_with_single_interspersed_simple_example", &test_parse_link_with_single_interspersed_simple_example));
	sut_suite_add_test(suite, sut_test_new("test_parse_link_with_single_interspersed_simple_titled_example", &test_parse_link_with_single_interspersed_simple_titled_example));
	sut_suite_add_test(suite, sut_test_new("test_parse_link_with_multiple_interspersed_simple_example", &test_parse_link_with_multiple_interspersed_simple_example));
	sut_suite_add_test(suite, sut_test_new("test_parse_link_with_multiple_interspersed_simple_titled_example", &test_parse_link_with_multiple_interspersed_simple_titled_example));
	sut_suite_add_test(suite, sut_test_new("test_parse_code_span_with_simple_example", &test_parse_code_span_with_simple_example));
	sut_suite_add_test(suite, sut_test_new("test_parse_single_interspersed_code_span", &test_parse_single_interspersed_code_span));
	sut_suite_add_test(suite, sut_test_new("test_parse_multiple_interspersed_code_span", &test_parse_multiple_interspersed_code_span));
	sut_suite_add_test(suite, sut_test_new("test_parse_simple_linebreak", &test_parse_simple_linebreak));
	sut_suite_add_test(suite, sut_test_new("test_parse_header1_atx", &test_parse_header1_atx));
	sut_suite_add_test(suite, sut_test_new("test_parse_header1_atx_with_trailing_delim", &test_parse_header1_atx_with_trailing_delim));
	sut_suite_add_test(suite, sut_test_new("test_parse_header1_setext", &test_parse_header1_setext));
	sut_suite_add_test(suite, sut_test_new("test_parse_header2_atx", &test_parse_header2_atx));
	sut_suite_add_test(suite, sut_test_new("test_parse_header2_setext", &test_parse_header2_setext));
	sut_suite_add_test(suite, sut_test_new("test_parse_header3_atx", &test_parse_header3_atx));
	sut_suite_add_test(suite, sut_test_new("test_parse_header4_atx", &test_parse_header4_atx));
	sut_suite_add_test(suite, sut_test_new("test_parse_header5_atx", &test_parse_header5_atx));
	sut_suite_add_test(suite, sut_test_new("test_parse_header6_atx", &test_parse_header6_atx));
	sut_suite_add_test(suite, sut_test_new("test_parse_block_code_indented_with_spaces", &test_parse_block_code_indented_with_spaces));
	sut_suite_add_test(suite, sut_test_new("test_parse_block_quote", &test_parse_block_quote));
	sut_suite_add_test(suite, sut_test_new("test_parse_text_with_paragraph", &test_parse_text_with_paragraph));
	sut_suite_add_test(suite, sut_test_new("test_parse_list", &test_parse_list));
	sut_suite_add_test(suite, sut_test_new("test_check_document_order", &test_check_document_order));
	sut_suite_run(suite, filter);
	sut_suite_free(suite);

	return 0;
}
