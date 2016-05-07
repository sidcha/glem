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
 *        File: glem/clientSample.c
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

void glcdServerSend(uint8_t *buf, int len)
{
	int glemFD;
	socklen_t sockLen;
	struct sockaddr_un servAddr;

	if ((glemFD = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("Failed at socket");
		return;
	}

	servAddr.sun_family = AF_UNIX;
	strcpy(servAddr.sun_path, "/tmp/glcdSocket");

	sockLen = sizeof(servAddr.sun_family) + strlen(servAddr.sun_path);

	if (connect(glemFD, (const struct sockaddr *)&servAddr, sockLen) != 0) {
		perror("Failed at connect");
		return;
	}

	int ret = write(glemFD, buf, len);
	if (ret <= 0) {
 		perror("Client write");
	}
	close(glemFD);
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("Usage: %s <widthPixel> <heightPixel>\n", argv[0]);
		exit (0);
	}
	int w = atoi(argv[1]);
	int h = atoi(argv[2]);
	if ( w <=0 || h <=0) {
		printf("Invalid input.\n");
		exit(-1);
	}
	uint8_t *buf = malloc (sizeof(uint8_t)* (w/8)*h);
	if (buf == NULL) {
		printf("Malloc Failed\n");
		exit (-1);
	}
	memset(buf, 0x55, (w/8)*h);
	glcdServerSend(buf, (w/8)*h);
	return 0;
}

