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

#define GLCD_BUFLEN	(GLCD_WIDTH/8*GLCD_HEIGHT)

#if defined (COL_MAJOR)
#define setPix(x, y)   glcdBuf[ ( (y>>3) * GLCD_WIDTH ) + x ] |=  (1<<(y&0x07))
#define clearPix(x, y) glcdBuf[ ( (y>>3) * GLCD_WIDTH ) + x ] &= ~(1<<(y&0x07))
#elif defined (ROW_MAJOR)
#define setPix(x, y)   glcdBuf[y * (GLCD_WIDTH >> 3) + (x >> 3)] |=  (0x0080 >> (x&0x0007))
#define clearPix(x, y) glcdBuf[y * (GLCD_WIDTH >> 3) + (x >> 3)] &= ~(0x0080 >> (x&0x0007))
#endif

uint8_t glcdBuf[GLCD_BUFLEN];

void glemServerSend(uint8_t *buf, int len)
{
	int glemFD;
	char path[64];
	socklen_t sockLen;
	struct sockaddr_un servAddr;

	if ((glemFD = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("GLEM: Failed at socket");
		return;
	}

	servAddr.sun_family = AF_UNIX;
	snprintf(path, 64, "/tmp/glcdSocket%dx%d", GLCD_WIDTH, GLCD_HEIGHT);
	path[63] = 0;
	strcpy(servAddr.sun_path, path);

	sockLen = sizeof(servAddr.sun_family) + strlen(servAddr.sun_path);

	if (connect(glemFD, (const struct sockaddr *)&servAddr, sockLen) != 0) {
		perror("GLEM: Failed at connect");
		return;
	}

	int ret = write(glemFD, buf, len);
	if (ret <= 0) {
 		perror("GLEM: Failed at write");
	}
	close(glemFD);
}

void glcdSetPixel(int x, int y, int color)
{
	if (x < 0 || x >= GLCD_WIDTH)
		return;
	if (y < 0 || y >= GLCD_HEIGHT)
		return;

	if (color)
		setPix(x, y);
	else
		clearPix(x, y); 
}

void glcdClear()
{
	memset(glcdBuf, 0x00, GLCD_BUFLEN);
	glemServerSend(glcdBuf, GLCD_BUFLEN);
}

void glcdRefresh()
{
	glemServerSend(glcdBuf, GLCD_BUFLEN);
}
