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
 *        File: glem.h
 *  Created on: 1-Jan-2017
 *      Author: Siddharth Chandrasekaran
 *        Mail: siddharth@embedjournal.com
 */


#ifndef _GLEM_H_
#define _GLEM_H_

#define GLEM_NO_FLAGS		0x00
#define GLEM_INIT_DONE		0x01

void glem_init(int glcd_width, int glcd_height, int flags);
void glem_server_send(uint8_t *buf, int len);
void glem_set_pixel(int x, int y, int color);

#endif

