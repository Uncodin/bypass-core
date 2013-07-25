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
#include "document.h"

using namespace Bypass;

static Document document;

void
test_document_initial_size()
{
	sut_assert(document.size() == 0);
}

void
test_document_append_single()
{
	Element element;

	document.append(element);
	sut_assert(document.size() == 1);
}

void
test_document_append_multiple()
{
	int i, count = 3;

	for (i = 0; i < count; i++) {
		Element element;
		char str[10];
		sprintf(str, "%d", i);
		element.setText(std::string(str));
		document.append(element);
	}

	sut_assert(document.size() == 3);
}

void
test_document_access_single()
{
	std::string expected = "expected";

	Element element;
	element.setText(expected);

	document.append(element);

	sut_assert(document[0].getText() == expected);
}

void
test_document_access_multiple()
{
	int i, count = 3;

	for (i = 0; i < count; i++) {
		Element element;

		char str[10];
		sprintf(str, "%d", i);

		const std::string value = std::string(str);

		element.setText(value);
		document.append(element);
	}

	sut_assert(document[0].getText() == "0");
	sut_assert(document[1].getText() == "1");
	sut_assert(document[2].getText() == "2");
}