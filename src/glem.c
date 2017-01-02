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
 *        File: glem/src/glem-server.c
 *  Created on: 07-May-2016
 *      Author: Siddharth Chandrasekaran
 *        Mail: siddharth@embedjournal.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/prctl.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <glem.h>

// Unix socket location
#define ADDRESS			"/tmp/glcdSocket"
#define GLEM_VER_MAJ		1
#define GLEM_VER_MIN		1
#define GL_PAD			100
#define glcd_get_pixel(a,x,y) (a[y * (glcd_width / 8) + (x / 8)] & (1 << (7 - x % 8)));
#define convert_local_to_glut(x,y) do { x = window_origin_x + (x * scale_factor); \
	y = window_origin_y + (y * scale_factor); } while(0)

// Globals
pid_t gl_pid;
uint8_t *glcd_frame;
uint8_t *glut_buf;
int server_sock;
int glcd_width;
int glcd_height;
int scale_factor;
int window_origin_x;
int window_origin_y;
int gl_width;
int gl_height;

void die(const char *note)
{
	printf("glem: Fatal, %s\n", note);
	exit(1);
}

void die_error(const char *note)
{
	char tmp[128];
	snprintf(tmp, 127, "glem: Fatal, %s", note);
	tmp[127] = 0;
	perror(tmp);
	exit(1);
}

void die_usage()
{
	printf( "\n"
		"Graphical LCD EMulator (GLEM v%d.%d)\n"
		"Tiny graphical LCD emulator for embedded platforms and other small projects that\n"
		"need a one-off GUI to display some data without having to write too much code.\n"
		"\n"
		"USAGE\n"
			"\tglem <-r RESOLUTION> [OPTIONS]\n"
		"OPTIONS\n"
			"\t-r RESOLUTION    -\t Resolution of GLCD WIDTHxHEIGHT (eg. 128x64)\n"
			"\t-s SCALE_FACTOR  -\t Pixel scale ratio. Default 1:2 (-s2)\n"
			"\t-t TEST_VAL      -\t Send some data to glem server to check connection\n"
			"\t-v VERBOSE       -\t Don't go into bacground\n"
			"\t-h HELP          -\t Display this help message\n"
		"EXAMPLE\n"
			"\t$ glem -r 128x64 -s3     # to start server\n"
			"\t$ glem -r 128x64 -t87    # to test a running server (draw vertical lines)\n"
		"\n", GLEM_VER_MAJ, GLEM_VER_MAJ
	);
	exit(-1);
}

void glut_set_pixel(int x, int y, int color)
{
	int i=0;
	for(i=0; i < 3; i++ ) {  //RGB Mirrors
		glut_buf[(y*gl_width+x)*3+i] = color;
	}
}

void glut_set_scaled_pixel(int x, int y, int color)
{
	int i, j, setVal;
	setVal = color ? 255 : 0;
	convert_local_to_glut(x, y);
	for (i=x; i<(x+scale_factor); i++) {
		for (j=y; j<(y+scale_factor); j++) {
			glut_set_pixel(i, j, setVal);
		}
	}
}

void make_glut_buf()
{
	int i, j, pix;
	for (i=0; i<glcd_width; i++) {
		for (j=0; j<glcd_height; j++) {
			pix = glcd_get_pixel(glcd_frame, i, j);
			glut_set_scaled_pixel(i, (glcd_height-1)-j, pix);
		}
	}
}

void glut_draw_screen()
{
	make_glut_buf();
	glDrawPixels(gl_width, gl_height, GL_RGB, GL_UNSIGNED_BYTE, glut_buf);
	glutSwapBuffers();
	glutPostRedisplay();
	usleep(500*1000);
}

void reshape(int x, int y)
{
	glViewport(0, 0, (GLsizei) gl_width+100, (GLsizei) gl_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, gl_width, 0.0, gl_height, -1.f, 1.f);
}

void draw_rect_thick(int x, int y, int w, int h, int tx, int ty, int color)
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
			glut_set_pixel(i, y+t, color);
			glut_set_pixel(i, y+h-1-t, color);
		}
	}
	for (i=y; i<y+h; i++) {
		/* Left and right sides */
		for (t=0; t<(tx); t++) {
			glut_set_pixel(x+t, i, color);
			glut_set_pixel(x+w-1-t, i, color);
		}
	}
}

void glut_pdeathsig_handler(int sigNum)
{
	shutdown(server_sock, SHUT_RDWR);
	close(server_sock);
	unlink(ADDRESS);
	exit(0);
}

void glut_init(int *argc, char **argv)
{
	char win_name[64];
	gl_pid = fork();
	if (gl_pid == (pid_t)(-1)) {
		die_error("Fork at glut_init");
	}
	if (gl_pid != 0) {
		// parent process.
		return;
	}
	glut_buf = malloc(sizeof(uint8_t)*gl_width*gl_height*3);
	if (glut_buf == NULL) {
		fprintf(stderr, "GLUT buf alloc failed\n");
		exit(-1);
	}
	// This thread reads from glcdFrame buffer and writes it 
	// into the GLUT window and is a pure consumer.
	prctl(PR_SET_PDEATHSIG, SIGHUP);
	signal(SIGHUP, glut_pdeathsig_handler);
	glutInitWindowSize(gl_width, gl_height);
	glutInit(argc, argv);
	glutReshapeFunc(reshape);
	glutIdleFunc(glut_draw_screen);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	snprintf(win_name, 64, "GLCD Emulator v%d.%d (%dx%d-%d)",GLEM_VER_MAJ,
			GLEM_VER_MIN, glcd_width, glcd_height, scale_factor);
	win_name[63]=0; // paranoia: force terminate.
	glutCreateWindow(win_name);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(255.0, 255.0, 255.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glClear(GL_POINT_BIT);
	draw_rect_thick(window_origin_x-2, window_origin_y-2, 
		(gl_width-GL_PAD+4), (gl_height-GL_PAD+4), 2, 2, 255);
	glutMainLoop();	// This is blocking!
	exit (0);	// Should never get here.
}

void glem_sigchld_handler(int sigNum)
{
	pid_t pid;
	if (gl_pid <= 0) // glem has not started.
		return;
	// Run through all the child processes to see if our
	// prodigal son has died. If he did, then glem quits.
	while ((pid = waitpid(gl_pid, NULL, WNOHANG)) != -1) {
		if (pid == gl_pid)
			exit(0);
	}
}

int glem_background()
{
	pid_t pid;
	if((pid = fork()) < 0) {
		die_error("fork: Error going into the background");
	}
	if(pid > 0) { // Is this the parent process? If so, end it.
		exit(0);
	}

	umask(0); // Mark this precess file creation mask to 0.

	if(setsid() < 0) { // Mark this child as session leader.
		die_error("setid: Error going into the background");
	}

	// Close all 3 file desc to prevent any output to terminal.
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	return(0);
}

void glem_test(int val)
{
	int size = glcd_width/8*glcd_height;
	uint8_t *data = (uint8_t *)malloc(size);
	if (data == NULL) {
		printf("glem: test out of memory!\n");
		return;
	}
	memset(data, val, size);
	glem_init(glcd_width, glcd_height, 0);
	glem_server_send(data, size);
}

int main(int argc, char *argv[])
{
	int glcd_buf_len;
	char path[64];
	uint8_t *tmp_read_buf;
	signal(SIGCHLD, glem_sigchld_handler);
	int opt, got_resolution=0, got_verbose=0, got_test=0, test_val=0;
	scale_factor = 2;
	while ((opt = getopt(argc, argv, "vhr:s:t:")) != -1) {
		switch(opt) {
		case 'r':
			if (sscanf(optarg, "%dx%d", &glcd_width, &glcd_height) < 2) {
				die_usage();
			}
			if (glcd_width < 10 || glcd_height < 10) {
				die("Unsupported resolution. Must be greater than 10x10");
			}
			got_resolution = 1;
			break;
		case 's':
			scale_factor = atoi(optarg);
			if (scale_factor == 0) {
				die_usage();
			}
			break;
		case 'v':
			got_verbose = 1;
			break;
		case 't':
			got_test = 1;
			test_val = atoi(optarg);
			break;
		case 'h':
		default:// '?'
			die_usage();
		}
	}

	if (!got_resolution) {		// Can't assume defaults here. Both these
		die_usage();		// are critical data. Hence mandated.
	}
	if (!got_verbose) {
		glem_background();
	}
	if (got_test) {
		// Send one off commands to an already running glem server for for
		// the given resolution. If a server is not running, glem_test will
		// fail with an error.
		glem_test(test_val);
		exit(0);
	}

	gl_width = glcd_width * scale_factor + GL_PAD;
	gl_height = glcd_height * scale_factor + GL_PAD;
	glcd_buf_len = (glcd_width/8) * glcd_height;
	window_origin_x = (gl_width/2)-((gl_width-GL_PAD)/2);
	window_origin_y = (gl_height/2)-((gl_height-GL_PAD)/2);

	glcd_frame = mmap(NULL, sizeof(uint8_t)*glcd_buf_len,
		PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (glcd_frame == NULL) {
		die("Shared memory alloc failed!");
	}
	tmp_read_buf = malloc(sizeof(uint8_t)*glcd_buf_len);
	if (tmp_read_buf == NULL) {
		die("Memory alloc failed!");
	}
	glut_init(&argc, argv);		// may modify argc and argv.

	if ((server_sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		die(strerror(errno));
	}
	struct sockaddr_un sock_serv;
	sock_serv.sun_family = AF_UNIX;
	snprintf(path, 64, "%s%dx%d", ADDRESS, glcd_width, glcd_height);
	strcpy(sock_serv.sun_path, path);
	unlink(path);

	socklen_t sock_len = sizeof(sock_serv.sun_family) + strlen(sock_serv.sun_path);

	if (bind(server_sock, (const struct sockaddr *)&sock_serv, sock_len) < 0) {
		die_error("Error Bind");
	}
	if (listen(server_sock, 5) < 0) {
		die_error("Error Listen");
	}
	while (1) {
		int client_fd, rec;
		struct sockaddr_un sock;
		socklen_t clen = sizeof(sock);
		client_fd = accept(server_sock, (struct sockaddr *)&sock, &clen);
		if (client_fd < 0) {
			perror("glem: failed at accpet");
			continue;
		}
		if ((rec = read(client_fd, tmp_read_buf, glcd_buf_len)) > 0) {
			printf("glem: receieved %d bytes from client\n", rec);
			// Race condition: The consumer theread may have been
			// reading when we call memcpy here causing tear. But
			// we can live with it.
			memcpy(glcd_frame, tmp_read_buf, rec);
		} else {
			// Read failed ??
			perror("glem: read failed");
		}
		close(client_fd);
	}
	// close left here for sytactic sugar.
	// the socket is closed in signal handler on exit.
	close(server_sock);
	return 0;
}

