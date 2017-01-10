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
 *        File: glem/src/glcd.c
 *  Created on: 07-May-2016
 *      Author: Siddharth Chandrasekaran
 *        Mail: siddharth@embedjournal.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>

#include <glem.h>

#include "glcd.h"
#include "objects.h"

#define set_pix_colmaj(x, y)   glcd_buf [ ( (y>>3) * glcd_width ) + x ] |=  (1<<(y&0x07))
#define clear_pix_colmaj(x, y) glcd_buf [ ( (y>>3) * glcd_width ) + x ] &= ~(1<<(y&0x07))
#define set_pix_rowmaj(x, y)   glcd_buf [y * (glcd_width >> 3) + (x >> 3)] |=  (0x0080 >> (x&0x0007))
#define clear_pix_rowmaj(x, y) glcd_buf [y * (glcd_width >> 3) + (x >> 3)] &= ~(0x0080 >> (x&0x0007))

int glcd_width;
int glcd_height;
int glcd_flags;
uint8_t *glcd_buf;

void glcd_clear()
{
	memset(glcd_buf, 0x00, glcd_width/8*glcd_height);
	glem_server_send(glcd_buf, glcd_width/8*glcd_height);
}

void glcd_refresh()
{
	glem_server_send(glcd_buf, glcd_width/8*glcd_height);
}

void glcd_init(int width, int height, int flags)
{
	glcd_width = width;
	glcd_height = height;
	glcd_flags = flags;
	glcd_buf = malloc (sizeof(uint8_t) * glcd_width/8 * glcd_height);
	if (glcd_buf == NULL) {
		printf("[ ! ] Error: glcd buffer alloc failed!\n");
		exit(-1);
	}
	printf("glem init..\n");
	glem_init(width, height, flags);
	printf("glem init done\n");
}

void glcd_set_pixel_direct(int x, int y, int color)
{
	glem_set_pixel(x, y, color);
}

void glcd_set_pixel(int x, int y, int color)
{
	if (x < 0 || x >= glcd_width)
		return;
	if (y < 0 || y >= glcd_height)
		return;

	if (glcd_flags & GLCD_COL_MAJOR) {
		if (color)
			set_pix_colmaj(x, y);
		else 
			clear_pix_colmaj(x, y); 
	} else {
		if (color)
			set_pix_rowmaj(x, y);
		else 
			clear_pix_rowmaj(x, y); 
	}
}

void glcd_draw_rowmaj(const uint8_t *bmp, int h, int w, int x, int y)
{
	int map = 0, byte=0;
	int xEnd = x + w;
	int yEnd = y + h;
	int bit = -1;
	while(1) {
		if(x == xEnd) {
			x -= w;
			bit = -1;	// Forcing new byte read in current iteration
			y++;
			if(y == yEnd)
				break;
		}
		if(bit == -1) {	// New byte read from font table
			bit = 7;
			map = bmp[byte++];
		}
		if(map & (1<<bit))
			glcd_set_pixel(x, y, 1);
		bit--;
		x++;
	}
}

void glcd_draw_colmaj(const uint8_t *bmp, int h, int w, int x, int y)
{
	int map = 0, byte=0;
	int xEnd = x + w;
	int yEnd = y + h;
	int bit = 8;
	while(1) {
		if(y == yEnd) {
			y -= h;
			bit = 8;
			x++;
			if(x == xEnd)
				break;
		}
		if(bit == 8) {
			bit = 0;
			map = bmp[byte++];
		}
		if(map & (1<<bit))
			glcd_set_pixel(x, y, 1);
		bit++;
		y++;
	}
}

int draw_char(const font_t *f, int c, int x, int y)
{
	int w, h;
	if (c < f->start_char || c > f->end_char)
		c = f->start_char;
	h = f->char_height;
	w = f->desc[c - f->start_char].char_width;
	uint8_t *bmp = (uint8_t *)f->bitmap;
	bmp += f->desc[c - f->start_char].offset;
	if (glcd_flags & GLCD_COL_MAJOR)
		glcd_draw_colmaj(bmp, h, w, x, y);
	else
		glcd_draw_rowmaj(bmp, h, w, x, y);
	return w;
}

int draw_string(const font_t *f, const char *s, int x, int y)
{
	int w;
	while (*s) {
		w = draw_char(f, *s, x, y);
		x += (w + 2);
		s++;
	}
	return x;
}

int draw_symbol(const symbol_t *s, int x, int y)
{
	if (glcd_flags & GLCD_COL_MAJOR)
		glcd_draw_colmaj(s->bitmap, s->height, s->width, x, y);
	else
		glcd_draw_rowmaj(s->bitmap, s->height, s->width, x, y);
	return s->width;
}

void probe_symbol(const symbol_t *s, int *w, int *h)
{
	*w = s->width;
	*h = s->height;
}

void probe_char(const font_t *f, int c, int *w, int *h)
{
	if (c < f->start_char || c > f->end_char)
		c = f->start_char;
	*h = f->char_height;
	*w = f->desc[c - f->start_char].char_width;
}

void probe_string(const font_t *f, const char *s, int *w, int *h)
{
	int tw, th, i=0;
	*w = 0; *h = 0;
	while (*s) {
		probe_char(f, *s, &tw, &th);
		*w += tw;
		s++; i++;
	}
	if (i != 0) { 
		*w += f->char_space*(i-1);
		*h = th;
	}
}
