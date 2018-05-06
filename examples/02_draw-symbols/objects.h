/******************************************************************************

                  Copyright (c) 2017 Siddharth Chandrasekaran

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

    Author : Siddharth Chandrasekaran
    Email  : siddharth@embedjournal.com
    Date   : 07-May-2016

******************************************************************************/

#ifndef __OBJECTS_H_
#define __OBJECTS_H_

#include <stdint.h>

#define OBJ_ROW_MAJOR 0x00
#define OBJ_COL_MAJOR 0x01

typedef struct {
	int char_width;
	int offset;
}font_descriptor_t;

typedef struct {
	const int char_space;
	const int char_height;
	const int start_char;
	const int end_char;
	const font_descriptor_t *desc;
	const int flags;
	const uint8_t *bitmap;
}font_t;

typedef struct {
	const int width;
	const int height;
	const int flags;
	const uint8_t *bitmap;
}symbol_t;

#endif
