/***************************************************************************
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 2 of the License, or       *
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
 *        File: glem/src/passfd.c
 *  Created on: 04-Jan-2017
 *      Author: Siddharth Chandrasekaran
 *        Mail: siddharth@embedjournal.com
 */

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <errno.h>

union fdmsg {
	struct cmsghdr h;
	char buf[CMSG_SPACE(sizeof(int))];
};

int sendfd(int sock_fd, int send_me_fd)
{
	int ret = 0;
	struct iovec  iov[1];
	struct msghdr msg;

	iov[0].iov_base = &ret; // Don't send any data.
	iov[0].iov_len  = 1;

	msg.msg_iov     = iov;
	msg.msg_iovlen  = 1;
	msg.msg_name    = 0;
	msg.msg_namelen = 0;

	union  fdmsg  cmsg;
	struct cmsghdr* h;

	msg.msg_control = cmsg.buf;
	msg.msg_controllen = sizeof(union fdmsg);
	msg.msg_flags = 0;

	h = CMSG_FIRSTHDR(&msg);
	h->cmsg_len   = CMSG_LEN(sizeof(int));
	h->cmsg_level = SOL_SOCKET;
	h->cmsg_type  = SCM_RIGHTS;
	*((int*)CMSG_DATA(h)) = send_me_fd;

	if ((ret = sendmsg(sock_fd, &msg, 0)) > 0)
		ret = 0; // return 0 on success.

	return ret;
}

int recvfd(int sock_fd, int *rec_sock_fd)
{
	int count;
	int ret = 0;
	struct iovec  iov[1];
	struct msghdr msg;

	iov[0].iov_base = &ret;  // don't receive any data
	iov[0].iov_len  = 1;

	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;

	union fdmsg  cmsg;
	struct cmsghdr* h;

	msg.msg_control = cmsg.buf;
	msg.msg_controllen = sizeof(union fdmsg);
	msg.msg_flags = 0;

	h = CMSG_FIRSTHDR(&msg);
	h->cmsg_len   = CMSG_LEN(sizeof(int));
	h->cmsg_level = SOL_SOCKET;  // Linux does not set these
	h->cmsg_type  = SCM_RIGHTS;  // upon return
	*((int*)CMSG_DATA(h)) = -1;

	if ((count = recvmsg(sock_fd, &msg, 0)) < 0) {
		ret = -1;
	} else {
		h = CMSG_FIRSTHDR(&msg);   // can realloc?
		if (   h == NULL
		    || h->cmsg_len    != CMSG_LEN(sizeof(int))
		    || h->cmsg_level  != SOL_SOCKET
		    || h->cmsg_type   != SCM_RIGHTS ) {
			ret = -1;
		} else {
			*rec_sock_fd = *((int*)CMSG_DATA(h));
			ret = 0;
		}
	}
	return ret;
}
