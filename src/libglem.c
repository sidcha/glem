/***************************************************************************
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

    File: glem/src/libglem.c
    Created: 01-Jan-2017
    Author: Siddharth Chandrasekaran <siddharth@embedjournal.com>

 **************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <glem.h>
#include "private.h"

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

static glem_command_t *glem_make_cmd_frame(uint8_t *frame, int frame_len)
{
	int total_len = sizeof(glem_command_t) + sizeof(struct glem_cmd_frame)+frame_len;
	uint8_t *data = malloc(total_len);
	if (data == NULL) {
		printf("glem: Out of memory\n");
		return NULL;
	}
	glem_command_t *cmd = (glem_command_t *)data;
	struct glem_cmd_frame *cmd_frame;
	cmd->cmd_num = GLEM_CMD_SEND_FRAME;
	cmd->data_len = sizeof(struct glem_cmd_frame) + frame_len;
	cmd_frame = (struct glem_cmd_frame *)cmd->data;
	cmd_frame->res_x = glem_glcd_width;
	cmd_frame->res_y = glem_glcd_height;
	cmd_frame->buf_len = frame_len;
	memcpy(cmd_frame->buf, frame, frame_len);
	return cmd;
}

static glem_command_t *glem_make_cmd_set_pixel(int x, int y, int color)
{
	int total_len = sizeof(glem_command_t) + sizeof(struct glem_cmd_set_pixel);
	uint8_t *data = malloc(total_len);
	if (data == NULL) {
		printf("glem: Out of memory\n");
		return NULL;
	}
	struct glem_cmd_set_pixel sp = {
		.pix_x = x,
		.pix_y = y,
		.pix_color = color,
	};
	glem_command_t *cmd = (glem_command_t *)data;
	cmd->cmd_num = GLEM_CMD_SET_PIX;
	cmd->data_len = sizeof(struct glem_cmd_set_pixel);
	memcpy(cmd->data, &sp, cmd->data_len);
	return cmd;
}

static int glem_connect()
{
	int fd;
	struct sockaddr_in server;

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 31337 );

	if ((glem_flags & GLEM_INIT_DONE) == 0) {
		printf("Error. glem not initialized!\n");
		return -1;
	}

	if ((fd = socket(AF_INET , SOCK_STREAM , 0)) == -1) {
		perror("glem: Failed at socket");
	}

	if (connect(fd , (struct sockaddr *)&server , sizeof(server)) < 0) {
		perror ("glem: Failed at connect to server");
		close(fd);
		return 1;
	}

	return fd;
}

void glem_write(uint8_t *buf, int len)
{
	int ret, glem_fd;
	glem_command_t *c= glem_make_cmd_frame(buf, len);
	if (c == NULL) {
		printf("glem: error creating command\n");
		return;
	}

	glem_fd = glem_connect();
	if (glem_fd > 0) {
		ret = write(glem_fd, c, sizeof(glem_command_t) + c->data_len);
		if (ret <= 0) {
			perror("glem: Failed at write");
		}
	}
	close(glem_fd);
	free(c);
}

void glem_setpix(int x, int y, int color)
{
	int ret, glem_fd;
	glem_command_t *c= glem_make_cmd_set_pixel(x, y, color);
	if (c == NULL) {
		printf("glem: error creating command\n");
		return;
	}

	glem_fd = glem_connect();
	if (glem_fd > 0) {
		ret = write(glem_fd, c, sizeof(glem_command_t) + c->data_len);
		if (ret <= 0) {
			perror("glem: Failed at write");
		}
	}
	close(glem_fd);
	free(c);
}

