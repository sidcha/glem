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
    Date   : 2-Jan-2017

******************************************************************************/

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

