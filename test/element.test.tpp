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
#include "element.h"
#include "sut_test.h"

using namespace Bypass;

static Element element;

void
test_initial_size()
{
	sut_assert(element.size() == 0);
}

void
test_block_code_type()
{
	element.setType(BLOCK_CODE);

	sut_assert(element.isBlockElement());
	sut_assert(!element.isSpanElement());
}

void
test_block_quote_type()
{
	element.setType(BLOCK_QUOTE);

	sut_assert(element.isBlockElement());
	sut_assert(!element.isSpanElement());
}

void
test_block_html_type()
{
	element.setType(BLOCK_HTML);

	sut_assert(element.isBlockElement());
	sut_assert(!element.isSpanElement());
}

void
test_header_type()
{
	element.setType(HEADER);

	sut_assert(element.isBlockElement());
	sut_assert(!element.isSpanElement());
}

void
test_hrule_type()
{
	element.setType(HRULE);

	sut_assert(element.isBlockElement());
	sut_assert(!element.isSpanElement());
}

void
test_list_type()
{
	element.setType(LIST);

	sut_assert(element.isBlockElement());
	sut_assert(!element.isSpanElement());
}

void
test_list_item_type()
{
	element.setType(LIST_ITEM);

	sut_assert(element.isBlockElement());
	sut_assert(!element.isSpanElement());
}

void
test_paragraph_type()
{
	element.setType(PARAGRAPH);

	sut_assert(element.isBlockElement());
	sut_assert(!element.isSpanElement());
}

void
test_table_type()
{
	element.setType(TABLE);

	sut_assert(element.isBlockElement());
	sut_assert(!element.isSpanElement());
}

void
test_table_cell_type()
{
	element.setType(TABLE_CELL);

	sut_assert(element.isBlockElement());
	sut_assert(!element.isSpanElement());
}

void
test_table_row_type()
{
	element.setType(TABLE_ROW);

	sut_assert(element.isBlockElement());
	sut_assert(!element.isSpanElement());
}

void
test_autolink_type()
{
	element.setType(AUTOLINK);

	sut_assert(!element.isBlockElement());
	sut_assert(element.isSpanElement());
}

void
test_code_span_type()
{
	element.setType(CODE_SPAN);

	sut_assert(!element.isBlockElement());
	sut_assert(element.isSpanElement());
}

void
test_double_emphasis_type()
{
	element.setType(DOUBLE_EMPHASIS);

	sut_assert(!element.isBlockElement());
	sut_assert(element.isSpanElement());
}

void
test_emphasis_type()
{
	element.setType(EMPHASIS);

	sut_assert(!element.isBlockElement());
	sut_assert(element.isSpanElement());
}

void
test_image_type()
{
	element.setType(IMAGE);

	sut_assert(!element.isBlockElement());
	sut_assert(element.isSpanElement());
}

void
test_linebreak_type()
{
	element.setType(LINEBREAK);

	sut_assert(!element.isBlockElement());
	sut_assert(element.isSpanElement());
}

void
test_link_type()
{
	element.setType(LINK);

	sut_assert(!element.isBlockElement());
	sut_assert(element.isSpanElement());
}

void
test_raw_html_type()
{
	element.setType(RAW_HTML_TAG);

	sut_assert(!element.isBlockElement());
	sut_assert(element.isSpanElement());
}

void
test_triple_emphasis_type()
{
	element.setType(TRIPLE_EMPHASIS);

	sut_assert(!element.isBlockElement());
	sut_assert(element.isSpanElement());
}

void
test_append_single()
{
	Element child;
	element.append(child);

	sut_assert(element.size() == 1);
}

void
test_access_single()
{
	std::string expected = "child";

	Element child;
	child.setText(expected);
	sut_assert(child.getText() == expected);

	element.append(child);

	sut_assert(element[0].getText() == expected);
}

void
test_append_multiple()
{
	int i, count = 3;

	for (i = 0; i < count; i++) {
		Element child;

		char str[10];
		sprintf(str, "%d", i);

		const std::string value = std::string(str);

		child.setText(value);
		element.append(child);
	}

	sut_assert(element[0].getText() == "0");
	sut_assert(element[1].getText() == "1");
	sut_assert(element[2].getText() == "2");
}

void
test_add_attribute()
{
	element.addAttribute("name", "value");
}

void
test_get_attribute()
{
	element.addAttribute("name", "value");
	sut_assert(element.getAttribute("name") == "value");
}

void
test_get_attribute_names_with_no_attributes()
{
	sut_assert(element.attrSize() == 0);
}

void
test_get_attribute_names_with_multiple_attributes()
{
	element.addAttribute("a", "A");
	element.addAttribute("b", "B");

	std::set<std::string> res;
	Element::AttributeMap::iterator it = element.attrBegin();
	for (; it != element.attrEnd(); ++it) {
		res.insert(it->first);
	}

	sut_assert(res.size() == 2);
	sut_assert(std::find(res.begin(), res.end(), "a") != res.end());
	sut_assert(std::find(res.begin(), res.end(), "b") != res.end());
}

