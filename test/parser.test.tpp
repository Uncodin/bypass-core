//
//  Copyright 2013 Uncodin, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include <string>
#include <stdio.h>
#include "parser.h"
#include "sut_test.h"

using namespace Bypass;

static Parser parser;

// Edge Cases ------------------------------------------------------------------

void
test_parse_null()
{
	Document document = parser.parse(NULL);
	sut_assert(document.size() == 0);
}

void
test_parse_empty()
{
	Document document = parser.parse(std::string(""));
	sut_assert(document.size() == 0);
}

// Text ------------------------------------------------------------------------

void
test_parse_text()
{
	Document document = parser.parse("hello world");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);

	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "hello world");
	sut_assert(document[0][0].size() == 0);
}

// Emphasis --------------------------------------------------------------------

void
test_parse_emphasis_with_simple_example()
{
	Document document = parser.parse("*hello world*");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == EMPHASIS);
	sut_assert(document[0][0].getText() == "hello world");
	sut_assert(document[0][0].size() == 0);
}

void
test_parse_single_interspersed_emphasis()
{
	Document document = parser.parse("one *two* three");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "one ");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == EMPHASIS);
	sut_assert(document[0][1].getText() == "two");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == TEXT);
	sut_assert(document[0][2].getText() == " three");
	sut_assert(document[0][2].size() == 0);
}

void
test_parse_multiple_interspersed_emphasis()
{
	Document document = parser.parse("*one* two *three*");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == EMPHASIS);
	sut_assert(document[0][0].getText() == "one");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == TEXT);
	sut_assert(document[0][1].getText() == " two ");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == EMPHASIS);
	sut_assert(document[0][2].getText() == "three");
	sut_assert(document[0][2].size() == 0);
}

// Double Emphasis -------------------------------------------------------------

void
test_parse_double_emphasis_with_simple_example()
{
	Document document = parser.parse("**hello world**");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == DOUBLE_EMPHASIS);
	sut_assert(document[0][0].getText() == "hello world");
	sut_assert(document[0][0].size() == 0);
}

void
test_parse_single_interspersed_double_emphasis()
{
	Document document = parser.parse("one **two** three");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "one ");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == DOUBLE_EMPHASIS);
	sut_assert(document[0][1].getText() == "two");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == TEXT);
	sut_assert(document[0][2].getText() == " three");
	sut_assert(document[0][2].size() == 0);
}

void
test_parse_multiple_interspersed_double_emphasis()
{
	Document document = parser.parse("**one** two **three**");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == DOUBLE_EMPHASIS);
	sut_assert(document[0][0].getText() == "one");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == TEXT);
	sut_assert(document[0][1].getText() == " two ");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == DOUBLE_EMPHASIS);
	sut_assert(document[0][2].getText() == "three");
	sut_assert(document[0][2].size() == 0);
}

// Triple Emphasis -------------------------------------------------------------

void
test_parse_triple_emphasis_with_simple_example()
{
	Document document = parser.parse("***hello world***");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == TRIPLE_EMPHASIS);
	sut_assert(document[0][0].getText() == "hello world");
	sut_assert(document[0][0].size() == 0);
}

void
test_parse_single_interspersed_triple_emphasis()
{
	Document document = parser.parse("one ***two*** three");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "one ");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == TRIPLE_EMPHASIS);
	sut_assert(document[0][1].getText() == "two");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == TEXT);
	sut_assert(document[0][2].getText() == " three");
	sut_assert(document[0][2].size() == 0);
}

void
test_parse_multiple_interspersed_triple_emphasis()
{
	Document document = parser.parse("***one*** two ***three***");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == TRIPLE_EMPHASIS);
	sut_assert(document[0][0].getText() == "one");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == TEXT);
	sut_assert(document[0][1].getText() == " two ");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == TRIPLE_EMPHASIS);
	sut_assert(document[0][2].getText() == "three");
	sut_assert(document[0][2].size() == 0);
}

// Strikethrough ---------------------------------------------------------------

void
test_parse_strikethrough_with_simple_example()
{
	Document document = parser.parse("~~hello world~~");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == STRIKETHROUGH);
	sut_assert(document[0][0].getText() == "hello world");
	sut_assert(document[0][0].size() == 0);
}

void
test_parse_single_interspersed_strikethrough()
{
	Document document = parser.parse("one ~~two~~ three");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "one ");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == STRIKETHROUGH);
	sut_assert(document[0][1].getText() == "two");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == TEXT);
	sut_assert(document[0][2].getText() == " three");
	sut_assert(document[0][2].size() == 0);
}

void
test_parse_multiple_interspersed_strikethrough()
{
	Document document = parser.parse("~~one~~ two ~~three~~");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == STRIKETHROUGH);
	sut_assert(document[0][0].getText() == "one");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == TEXT);
	sut_assert(document[0][1].getText() == " two ");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == STRIKETHROUGH);
	sut_assert(document[0][2].getText() == "three");
	sut_assert(document[0][2].size() == 0);
}

// Link ------------------------------------------------------------------------

void
test_parse_link_with_simple_example()
{
	Document document = parser.parse("[one](http://example.net/)");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == LINK);
	sut_assert(document[0][0].getText() == "one");
	sut_assert(document[0][0].getAttribute("link") == "http://example.net/");
	sut_assert(document[0][0].attrSize() == 1);
	sut_assert(document[0][0].size() == 0);
}

void
test_parse_link_with_simple_titled_example()
{
	Document document = parser.parse("[one](http://example.net/ \"One\")");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == LINK);
	sut_assert(document[0][0].getText() == "one");
	sut_assert(document[0][0].getAttribute("link") == "http://example.net/");
	sut_assert(document[0][0].getAttribute("title") == "One");
	sut_assert(document[0][0].attrSize() == 2);
	sut_assert(document[0][0].size() == 0);
}

void
test_parse_link_with_single_interspersed_simple_example()
{
	Document document = parser.parse("one [two](http://example.net/) three");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "one ");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == LINK);
	sut_assert(document[0][1].getText() == "two");
	sut_assert(document[0][1].getAttribute("link") == "http://example.net/");
	sut_assert(document[0][1].attrSize() == 1);
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == TEXT);
	sut_assert(document[0][2].getText() == " three");
	sut_assert(document[0][2].size() == 0);
}

void
test_parse_link_with_single_interspersed_simple_titled_example()
{
	Document document = parser.parse(
			"one [two](http://example.net/ \"Two\") three");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "one ");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == LINK);
	sut_assert(document[0][1].getText() == "two");
	sut_assert(document[0][1].getAttribute("link") == "http://example.net/");
	sut_assert(document[0][1].getAttribute("title") == "Two");
	sut_assert(document[0][1].attrSize() == 2);
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == TEXT);
	sut_assert(document[0][2].getText() == " three");
	sut_assert(document[0][2].size() == 0);
}

void
test_parse_link_with_multiple_interspersed_simple_example()
{
	Document document = parser.parse(
			"[one](http://example.net/) two [three](http://example.net/)");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == LINK);
	sut_assert(document[0][0].getText() == "one");
	sut_assert(document[0][0].getAttribute("link") == "http://example.net/");
	sut_assert(document[0][0].attrSize() == 1);
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == TEXT);
	sut_assert(document[0][1].getText() == " two ");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == LINK);
	sut_assert(document[0][2].getText() == "three");
	sut_assert(document[0][2].getAttribute("link") == "http://example.net/");
	sut_assert(document[0][2].attrSize() == 1);
	sut_assert(document[0][2].size() == 0);
}

void
test_parse_link_with_multiple_interspersed_simple_titled_example()
{
	Document document =
			parser.parse(
					"[one](http://example.net/ \"One\") two [three](http://example.net/ \"Three\")");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == LINK);
	sut_assert(document[0][0].getText() == "one");
	sut_assert(document[0][0].getAttribute("link") == "http://example.net/");
	sut_assert(document[0][0].getAttribute("title") == "One");
	sut_assert(document[0][0].attrSize() == 2);
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == TEXT);
	sut_assert(document[0][1].getText() == " two ");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == LINK);
	sut_assert(document[0][2].getText() == "three");
	sut_assert(document[0][2].getAttribute("link") == "http://example.net/");
	sut_assert(document[0][2].getAttribute("title") == "Three");
	sut_assert(document[0][2].attrSize() == 2);
	sut_assert(document[0][2].size() == 0);
}

// Code Span -------------------------------------------------------------------

void
test_parse_code_span_with_simple_example()
{
	Document document = parser.parse("`one`");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == CODE_SPAN);
	sut_assert(document[0][0].getText() == "one");
	sut_assert(document[0][0].size() == 0);
}

void
test_parse_single_interspersed_code_span()
{
	Document document = parser.parse("one `two` three");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "one ");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == CODE_SPAN);
	sut_assert(document[0][1].getText() == "two");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == TEXT);
	sut_assert(document[0][2].getText() == " three");
	sut_assert(document[0][2].size() == 0);
}

void
test_parse_multiple_interspersed_code_span()
{
	Document document = parser.parse("`one` two `three`");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == CODE_SPAN);
	sut_assert(document[0][0].getText() == "one");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == TEXT);
	sut_assert(document[0][1].getText() == " two ");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == CODE_SPAN);
	sut_assert(document[0][2].getText() == "three");
	sut_assert(document[0][2].size() == 0);
}

// Line Break ------------------------------------------------------------------
void
test_parse_simple_linebreak()
{
	Document document = parser.parse("one  \ntwo");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].getText().length() == 0);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "one");
	sut_assert(document[0][0].size() == 0);
	sut_assert(document[0][1].getType() == LINEBREAK);
	sut_assert(document[0][1].getText() == "");
	sut_assert(document[0][1].size() == 0);
	sut_assert(document[0][2].getType() == TEXT);
	sut_assert(document[0][2].getText() == "two");
	sut_assert(document[0][2].size() == 0);
}

// Header ----------------------------------------------------------------------

void
test_parse_header1_atx()
{
	Document document = parser.parse("# one");
	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == HEADER);
	sut_assert(document[0].getAttribute("level") == "1");
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "one");
}

void
test_parse_header1_atx_with_trailing_delim()
{
	Document document = parser.parse("# one #");
	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == HEADER);
	sut_assert(document[0].getAttribute("level") == "1");
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "one");
}

void
test_parse_header1_setext()
{
	Document document = parser.parse("one\n===");
	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == HEADER);
	sut_assert(document[0].getAttribute("level") == "1");
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "one");
}

void
test_parse_header2_atx()
{
	Document document = parser.parse("## two");
	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == HEADER);
	sut_assert(document[0].getAttribute("level") == "2");
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "two");
}

void
test_parse_header2_setext()
{
	Document document = parser.parse("two\n---");
	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == HEADER);
	sut_assert(document[0].getAttribute("level") == "2");
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "two");
}

void
test_parse_header3_atx()
{
	Document document = parser.parse("### three");
	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == HEADER);
	sut_assert(document[0].getAttribute("level") == "3");
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "three");
}

void
test_parse_header4_atx()
{
	Document document = parser.parse("#### four");
	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == HEADER);
	sut_assert(document[0].getAttribute("level") == "4");
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "four");
}

void
test_parse_header5_atx()
{
	Document document = parser.parse("##### five");
	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == HEADER);
	sut_assert(document[0].getAttribute("level") == "5");
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "five");
}

void
test_parse_header6_atx()
{
	Document document = parser.parse("###### six");
	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == HEADER);
	sut_assert(document[0].getAttribute("level") == "6");
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "six");
}

// Block code ------------------------------------------------------------------

void
test_parse_block_code_indented_with_spaces()
{
	Document document = parser.parse("This is a normal paragraph:\n"
			"\n"
			"    This is a code block.");
	sut_assert(document.size() == 2);
	sut_assert(document[0].getType() == PARAGRAPH);
	sut_assert(document[0].size() == 1);
	sut_assert(document[0][0].getType() == TEXT);
	sut_assert(document[0][0].getText() == "This is a normal paragraph:");
	sut_assert(document[1].getType() == BLOCK_CODE);
	sut_assert(document[1].size() == 1);
	sut_assert(document[1][0].getType() == TEXT);
	sut_assert(document[1][0].getText() == "This is a code block.");
}

// Block Quote ---------------------------------------------------------------

void
test_parse_block_quote()
{
	Document document = parser.parse("> ## This is a header.\n"
			">\n"
			"> -\tListItem\n"
			"> -\tListItem2\n"
			">\n"
			">More Text!\n");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == BLOCK_QUOTE);
	sut_assert(document[0].size() == 3);
	sut_assert(document[0][0].getType() == HEADER);
	sut_assert(document[0][1].getType() == LIST);
	sut_assert(document[0][2].getType() == PARAGRAPH);
}

// Advanced -------------------------------------------------------------------

void
test_parse_text_with_paragraph()
{
	Document document =
			parser.parse(
					"Credits\n"
					"-------\n"
					"\n"
					"`Sundown` is based on the original Upskirt parser by Natacha Port\u00e9, with many additions\n"
					"by Vicent Marti (@vmg) and contributions from the following authors:"
					"\n\n"
					"\tBen Noordhuis, Bruno Michel, Joseph Koshy, Krzysztof Kowalczyk, Samuel Bronson,\n"
					"\tShuhei Tanuma");

	sut_assert(document.size() == 3);
	sut_assert(document[0].getType() == HEADER);
	sut_assert(document[1].getType() == PARAGRAPH);
	sut_assert(document[2].getType() == BLOCK_CODE);
}

void
test_parse_list()
{
	Document document =
			parser.parse(
					"*\t**Fully standards compliant**\n"
					"\n"
					"\t`Sundown` passes out of the box the official Markdown v1.0.0 and v1.0.3\n"
					"\ttest suites, and has been extensively tested with additional corner cases\n"
					"\tto make sure its output is as sane as possible at all times.\n"
					"\n"
					"*\t**Massive extension support**\n"
					"\n"
					"\t`Sundown` has optional support for several (unofficial) Markdown extensions,\n"
					"\tsuch as non-strict emphasis(enced code blocks, tables, autolinks,\n"
					"\tstrikethrough and more.");

	sut_assert(document.size() == 1);
	sut_assert(document[0].getType() == LIST);
	sut_assert(document[0].size() == 2);
	sut_assert(document[0][0].getType() == LIST_ITEM);
	sut_assert(document[0][0].size() == 2);
	sut_assert(document[0][0][0].getType() == PARAGRAPH);
	sut_assert(document[0][0][0][0].getType() == DOUBLE_EMPHASIS);
	sut_assert(document[0][0][1].getType() == PARAGRAPH);
	sut_assert(document[0][0][1][0].getType() == CODE_SPAN);
	sut_assert(document[0][1].getType() == LIST_ITEM);
	sut_assert(document[0][1].size() == 2);
	sut_assert(document[0][1][0].getType() == PARAGRAPH);
	sut_assert(document[0][1][0][0].getType() == DOUBLE_EMPHASIS);
	sut_assert(document[0][1][1].getType() == PARAGRAPH);
	sut_assert(document[0][1][1][0].getType() == CODE_SPAN);
}

// Document Order -----------------------------------------------------

void
test_check_document_order()
{
	Document document = parser.parse("Header!!\n--------\nText goes here or something\n*One* **Two** ***Three***\n");
	sut_assert(document.size() == 2);
	sut_assert(document[0].getType() == HEADER);
	sut_assert(document[1].getType() == PARAGRAPH);
}
