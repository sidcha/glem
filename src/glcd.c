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
 *        File: glem/sample/glcd.c
 *  Created on: 07-May-2016
 *      Author: Siddharth Chandrasekaran
 *        Mail: siddharth3141@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>

#include "glcd.h"

#if defined (COL_MAJOR)
#define set_pix(x, y)   glcd_buf [ ( (y>>3) * glcd_width ) + x ] |=  (1<<(y&0x07))
#define clear_pix(x, y) glcd_buf [ ( (y>>3) * glcd_width ) + x ] &= ~(1<<(y&0x07))
#elif defined (ROW_MAJOR)
#define set_pix(x, y)   glcd_buf [y * (glcd_width >> 3) + (x >> 3)] |=  (0x0080 >> (x&0x0007))
#define clear_pix(x, y) glcd_buf [y * (glcd_width >> 3) + (x >> 3)] &= ~(0x0080 >> (x&0x0007))
#endif

int glcd_width;
int glcd_height;
uint8_t *glcd_buf;

void glem_server_send(uint8_t *buf, int len)
{
	int glem_fd;
	char path[64];
	socklen_t sock_len;
	struct sockaddr_un serv_addr;

	if ((glem_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("GLEM: Failed at socket");
		return;
	}

	serv_addr.sun_family = AF_UNIX;
	snprintf(path, 64, "/tmp/glcdSocket%dx%d", glcd_width, glcd_height);
	path[63] = 0;
	strcpy(serv_addr.sun_path, path);

	sock_len = sizeof(serv_addr.sun_family) + strlen(serv_addr.sun_path);

	if (connect(glem_fd, (const struct sockaddr *)&serv_addr, sock_len) != 0) {
		perror("GLEM: Failed at connect");
		printf("Attempt to connect to %s\n", path);
		return;
	}

	int ret = write(glem_fd, buf, len);
	if (ret <= 0) {
 		perror("GLEM: Failed at write");
	}
	close(glem_fd);
}

void glcd_init(int width, int height)
{
	glcd_width = width;
	glcd_height = height;

	glcd_buf = malloc (sizeof(uint8_t) * glcd_width/8 * glcd_height);
	if (glcd_buf == NULL) {
		printf("[ ! ] Error: glcd buffer alloc failed!\n");
		exit(-1);
	}
}

void glcd_set_pixel(int x, int y, int color)
{
	if (x < 0 || x >= glcd_width)
		return;
	if (y < 0 || y >= glcd_height)
		return;

	if (color)
		set_pix(x, y);
	else
		clear_pix(x, y); 
}

void glcd_clear()
{
	memset(glcd_buf, 0x00, glcd_width/8*glcd_height);
	glem_server_send(glcd_buf, glcd_width/8*glcd_height);
}

void glcd_refresh()
{
	glem_server_send(glcd_buf, glcd_width/8*glcd_height);
}
