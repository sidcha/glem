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

    File: glem/src/private.h
    Created: 2-Jan-2017
    Author: Siddharth Chandrasekaran <siddharth@embedjournal.com>

 **************************************************************************/


#ifndef _PRIVATE_H_
#define _PRIVATE_H_

enum glem_command_e {
	GLEM_CMD_SET_PIX,
	GLEM_CMD_SEND_FRAME,
	GLEM_CMD_CLEAR_SCREEN,
	GLEM_CMD_SENTINEL
};

typedef struct {
	enum glem_command_e cmd_num;
	int data_len;
	uint8_t data[];
} glem_command_t;

struct glem_cmd_frame {
	int res_x;
	int res_y;
	int buf_len;
	uint8_t buf[];
};

struct glem_cmd_set_pixel {
	int pix_x;
	int pix_y;
	int pix_color;
};

#endif

