#include <stdlib.h>
#include <getopt.h>
#include "sut_test.h"
#include "element.test.tpp"

int
main(int argc, char *argv[])
{
	sut_suite_t *suite;
	int ch;
	const char *filter = NULL;

	suite = sut_suite_new("element");


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

	sut_suite_add_test(suite, sut_test_new("test_initial_size", &test_initial_size));
	sut_suite_add_test(suite, sut_test_new("test_block_code_type", &test_block_code_type));
	sut_suite_add_test(suite, sut_test_new("test_block_quote_type", &test_block_quote_type));
	sut_suite_add_test(suite, sut_test_new("test_block_html_type", &test_block_html_type));
	sut_suite_add_test(suite, sut_test_new("test_header_type", &test_header_type));
	sut_suite_add_test(suite, sut_test_new("test_hrule_type", &test_hrule_type));
	sut_suite_add_test(suite, sut_test_new("test_list_type", &test_list_type));
	sut_suite_add_test(suite, sut_test_new("test_list_item_type", &test_list_item_type));
	sut_suite_add_test(suite, sut_test_new("test_paragraph_type", &test_paragraph_type));
	sut_suite_add_test(suite, sut_test_new("test_table_type", &test_table_type));
	sut_suite_add_test(suite, sut_test_new("test_table_cell_type", &test_table_cell_type));
	sut_suite_add_test(suite, sut_test_new("test_table_row_type", &test_table_row_type));
	sut_suite_add_test(suite, sut_test_new("test_autolink_type", &test_autolink_type));
	sut_suite_add_test(suite, sut_test_new("test_code_span_type", &test_code_span_type));
	sut_suite_add_test(suite, sut_test_new("test_double_emphasis_type", &test_double_emphasis_type));
	sut_suite_add_test(suite, sut_test_new("test_emphasis_type", &test_emphasis_type));
	sut_suite_add_test(suite, sut_test_new("test_image_type", &test_image_type));
	sut_suite_add_test(suite, sut_test_new("test_linebreak_type", &test_linebreak_type));
	sut_suite_add_test(suite, sut_test_new("test_link_type", &test_link_type));
	sut_suite_add_test(suite, sut_test_new("test_raw_html_type", &test_raw_html_type));
	sut_suite_add_test(suite, sut_test_new("test_triple_emphasis_type", &test_triple_emphasis_type));
	sut_suite_add_test(suite, sut_test_new("test_append_single", &test_append_single));
	sut_suite_add_test(suite, sut_test_new("test_access_single", &test_access_single));
	sut_suite_add_test(suite, sut_test_new("test_append_multiple", &test_append_multiple));
	sut_suite_add_test(suite, sut_test_new("test_add_attribute", &test_add_attribute));
	sut_suite_add_test(suite, sut_test_new("test_get_attribute", &test_get_attribute));
	sut_suite_add_test(suite, sut_test_new("test_get_attribute_names_with_no_attributes", &test_get_attribute_names_with_no_attributes));
	sut_suite_add_test(suite, sut_test_new("test_get_attribute_names_with_multiple_attributes", &test_get_attribute_names_with_multiple_attributes));
	sut_suite_run(suite, filter);
	sut_suite_free(suite);

	return 0;
}
