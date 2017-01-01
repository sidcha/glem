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
 *        File: glem/src/glem-dev.c
 *  Created on: 01-Jan-2017
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

static int glem_glcd_width;
static int glem_glcd_height;
static int glem_flags;

void glem_init(int glcd_width, int glcd_height, int flags)
{
	glem_glcd_width = glcd_width;
	glem_glcd_height = glcd_height;
	glem_flags = flags;
	glem_flags |= GLEM_INIT_DONE;
}

void glem_server_send(uint8_t *buf, int len)
{
	if ((glem_flags & GLEM_INIT_DONE) == 0) {
		printf("Error. glem not initialized!\n");
	}

	int glem_fd;
	char path[64];
	socklen_t sock_len;
	struct sockaddr_un serv_addr;

	if ((glem_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("glem: Failed at socket");
		return;
	}

	serv_addr.sun_family = AF_UNIX;
	snprintf(path, 64, "/tmp/glcdSocket%dx%d", 
			glem_glcd_width, glem_glcd_height);
	path[63] = 0;
	strcpy(serv_addr.sun_path, path);

	sock_len = sizeof(serv_addr.sun_family) + strlen(serv_addr.sun_path);

	if (connect(glem_fd, (const struct sockaddr *)&serv_addr, sock_len) == 0) {
		int ret = write(glem_fd, buf, len);
		if (ret <= 0)
			perror("glem: Failed at write");
	} else {
		perror ("glem: Failed at connect to server");
		printf("Attempt to connect to %s\n", path);
	}
	close(glem_fd);
}

