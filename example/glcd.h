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
void glcd_clear();
void glcd_refresh();

int draw_char(const font_t *f, int c, int x, int y);
int draw_string(const font_t *f, const char *s, int x, int y);
int draw_symbol(const symbol_t *s, int x, int y);
void probe_string(const font_t *f, const char *s, int *w, int *h);
void probe_symbol(const symbol_t *s, int *w, int *h);

#endif

