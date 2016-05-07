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
 *        File: glem/main.c
 *  Created on: 07-May-2016
 *      Author: Siddharth Chandrasekaran
 *        Mail: siddharth3141@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <signal.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/prctl.h>

#include <GL/glut.h>
#include <GL/gl.h>

// The GLCD width and height in pixel
#define GLCD_WIDTH_PIX		240
#define GLCD_HEIGHT_PIX		128

// Unix socket location
#define ADDRESS			"/tmp/glcdSocket"

// On high res monitors, a small GLCDs may look very
// small. Scale factor is used to scale up the size.
// In this case it is scaled up 4 times.
#define SCALE_FACTOR		4

// Internal Macros
#define GLCD_BUF_LEN		((GLCD_WIDTH_PIX/8)*GLCD_HEIGHT_PIX)
#define GL_WIDTH		(GLCD_WIDTH_PIX * SCALE_FACTOR + 100)
#define GL_HEIGHT		(GLCD_HEIGHT_PIX * SCALE_FACTOR + 100)
#define GL_GLCD_SCALED_WIDTH	(GLCD_WIDTH_PIX*SCALE_FACTOR)
#define GL_GLCD_SCALED_HEIGHT	(GLCD_HEIGHT_PIX*SCALE_FACTOR)
#define GL_GLCD_ORIGIN_X	((GL_WIDTH/2)-(GL_GLCD_SCALED_WIDTH/2))
#define GL_GLCD_ORIGIN_Y	((GL_HEIGHT/2)-(GL_GLCD_SCALED_HEIGHT/2))
#define glcdGetPixel(a,x,y) (a[y * (GLCD_WIDTH_PIX / 8) + (x / 8)] & (1 << (7 - x % 8)));
#define convertLocalToGL(x,y) do { x = GL_GLCD_ORIGIN_X + (x * SCALE_FACTOR); \
	y = GL_GLCD_ORIGIN_Y + (y * SCALE_FACTOR); } while(0)

// Globals
uint8_t *glcdFrame;
uint8_t glutBuf[GL_WIDTH * GL_HEIGHT * 3];
int gListeningSocket;

void glutSetPixel(int x, int y, int color)
{
	int i=0;
	for(i=0; i < 3; i++ ) {  //RGB Mirrors
		glutBuf[(y*GL_WIDTH+x)*3+i] = color;
	}
}

void glutSetScaledPixel(int x, int y, int color)
{
	int i, j, setVal=0;
	if (color)
		setVal = 255;
	convertLocalToGL(x, y);
	for (i=x; i<(x+SCALE_FACTOR); i++) {
		for (j=y; j<(y+SCALE_FACTOR); j++) {
			glutSetPixel(i, j, setVal);
		}
	}
}

void makeGLUTBuf()
{
	int i, j, pix;
	for (i=0; i<GLCD_WIDTH_PIX; i++) {
		for (j=0; j<GLCD_HEIGHT_PIX; j++) {
			pix = glcdGetPixel(glcdFrame, i, j);
			glutSetScaledPixel(i,(GLCD_HEIGHT_PIX-1)-j, pix);
		}
	}
}

void glutDrawScreen()
{
	makeGLUTBuf();
	glDrawPixels(GL_WIDTH, GL_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, glutBuf);
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int x, int y)
{
	glViewport(0, 0, (GLsizei) GL_WIDTH, (GLsizei) GL_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, GL_WIDTH, 0.0, GL_HEIGHT, -1.f, 1.f);
}

void glutDrawRectThick(int x, int y, int w, int h, int tx, int ty, int color)
{
	int i, t;

	if (tx == 0) {
		tx = 1;
	}

	if (ty == 0) {
		ty = 1;
	}

	for (i=x; i<x+w; i++) {
		/* Top and bottom sides */
		for (t=0; t<(ty); t++) {
			glutSetPixel(i, y+t, color);
			glutSetPixel(i, y+h-1-t, color);
		}
	}
	for (i=y; i<y+h; i++) {
		/* Left and right sides */
		for (t=0; t<(tx); t++) {
			glutSetPixel(x+t, i, color);
			glutSetPixel(x+w-1-t, i, color);
		}
	}
}

void glutSigHandler(int sigNum)
{
	shutdown(gListeningSocket, SHUT_RDWR);
	close(gListeningSocket);
	unlink(ADDRESS);
	exit(0);
}

void glemSigHandler(int sigNum)
{
	exit(0);
}

void glcdInit(int *argc, char **argv)
{
	int pid;
	glcdFrame = mmap(NULL, sizeof(uint8_t)*GLCD_BUF_LEN,
			PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (glcdFrame == NULL) {
		printf("GLCD alloc Failed!!\n");
		exit(-1);
	}
	if ((pid = fork()) != 0) {
		// parent process.
		return;
	}

	// This thread reads from glcdFrame buffer and writes it 
	// into the GLUT window. This is a pure consumer.

	prctl(PR_SET_PDEATHSIG, SIGINT);
	signal(SIGINT, glutSigHandler);
	glutInitWindowSize(GL_WIDTH, GL_HEIGHT);
	glutInit(argc, argv);
	glutReshapeFunc(reshape);
	glutIdleFunc(glutDrawScreen);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("GLCD Emulator v0.01");
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(255.0, 255.0, 255.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glClear(GL_POINT_BIT);
	glutDrawRectThick(GL_GLCD_ORIGIN_X-2, GL_GLCD_ORIGIN_Y-2,
		GL_GLCD_SCALED_WIDTH+4, GL_GLCD_SCALED_HEIGHT+4, 2, 2, 255);
	// This is blocking!
	glutMainLoop();
	// Should never get here.
}


int main(int argc, char *argv[])
{
	signal(SIGINT, glemSigHandler);
	glcdInit(&argc, argv);
	if ((gListeningSocket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("server: socket");
		exit(1);
	}

	struct sockaddr_un sockServ;
	sockServ.sun_family = AF_UNIX;
	strcpy(sockServ.sun_path, ADDRESS);
	unlink(ADDRESS);
	socklen_t sockLen = sizeof(sockServ.sun_family) + strlen(sockServ.sun_path);
	if (bind(gListeningSocket, (const struct sockaddr *)&sockServ, sockLen) < 0) {
		perror("server: bind");
		exit(1);
	}
	if (listen(gListeningSocket, 5) < 0) {
		perror("server: listen");
		exit(1);
	}

	while (1) {
		int clientFD, rec;
		struct sockaddr_un clientSock;
		socklen_t clientLen = sizeof(clientSock);
		if ((clientFD = accept(gListeningSocket, (struct sockaddr *)&clientSock, &clientLen)) < 0) {
			perror("server: accept");
			exit(1);
		}
		uint8_t tmp[GLCD_BUF_LEN];
		if ((rec = read(clientFD, tmp, GLCD_BUF_LEN)) < 0) {
			perror("[ ! ] ERROR reading from socket");
			exit(-1);
		}
		printf("Rec: %d bytes from client\n", rec);
		// Race condition: The consumer theread may have been
		// reading when we call memcpy here causing tear. But
		// that's something we can't live with.
		memcpy(glcdFrame, tmp, GLCD_BUF_LEN);
		close(clientFD);
	}
	// close left here for sytactic sugar.
	// the socket is closed in signal handler on exit.
	close(gListeningSocket);
	return 0;
}

