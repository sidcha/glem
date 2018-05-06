/***************************************************************************
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 **************************************************************************/

/*
 *        File: glem/src/glcd.h
 *  Created on: 07-May-2016
 *      Author: Siddharth Chandrasekaran
 *        Mail: siddharth@embedjournal.com
 */


#ifndef _GLCD_H_
#define _GLCD_H_

#include "objects.h"

#define GLCD_ROW_MAJOR 0x00
#define GLCD_COL_MAJOR 0x01

void glcd_init(int width, int height, int flags);
void glcd_set_pixel(int x, int y, int color);
void glcd_set_pixel_direct(int x, int y, int color);
void glcd_clear();
void glcd_refresh();

int draw_char(const font_t *f, int c, int x, int y);
int draw_string(const font_t *f, const char *s, int x, int y);
int draw_symbol(const symbol_t *s, int x, int y);
void probe_string(const font_t *f, const char *s, int *w, int *h);
void probe_symbol(const symbol_t *s, int *w, int *h);

#endif

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
    Date   : Thu Oct 19 06:02:01 IST 2017

******************************************************************************/
