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
 *        File: glem/src/passfd.h
 *  Created on: 04-Jan-2017
 *      Author: Siddharth Chandrasekaran
 *        Mail: siddharth@embedjournal.com
 */

#ifndef PASSFD_H_
#define PASSFD_H_

int sendfd(int sock_fd, int send_me_fd);
int recvfd(int sock_fd, int *rec_sock_fd);

#endif /* PASSDF_H_ */
