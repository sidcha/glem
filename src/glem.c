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
    Date   : 07-May-2016

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <glem.h>
#include "private.h"

#define GLEM_VER_MAJ		1
#define GLEM_VER_MIN		2
#define GL_PAD			100
#define glcd_get_pixel(a,x,y) (a[y * (glcd_width / 8) + (x / 8)] & (1 << (7 - x % 8)));
#define convert_local_to_glut(x,y) do { x = window_origin_x + (x * scale_factor); \
	y = window_origin_y + (y * scale_factor); } while(0)

#define set_pix_colmaj(x, y)   glcd_buf [ ( (y>>3) * glcd_width ) + x ] |=  (1<<(y&0x07))
#define clear_pix_colmaj(x, y) glcd_buf [ ( (y>>3) * glcd_width ) + x ] &= ~(1<<(y&0x07))
#define set_pix_rowmaj(x, y)   glcd_buf [y * (glcd_width >> 3) + (x >> 3)] |=  (0x0080 >> (x&0x0007))
#define clear_pix_rowmaj(x, y) glcd_buf [y * (glcd_width >> 3) + (x >> 3)] &= ~(0x0080 >> (x&0x0007))

uint8_t *glcd_buf;   // Holds the low res bit map.
int glcd_width;
int glcd_height;
int gl_width;
int gl_height;
int window_origin_x;
int window_origin_y;
int scale_factor;
int redisplay_interval;

void glem_usage()
{
	printf( "\n"
		"glem - Graphical LCD EMulator v%d.%d\n"
		"\n"
		"USAGE\n"
			"\tglem <-r RESOLUTION> [OPTIONS]\n"
		"OPTIONS\n"
			"\t-r RESOLUTION -\t Resolution of GLCD WIDTHxHEIGHT (eg. 128x64)\n"
			"\t-s SCALE      -\t Pixel scale ratio. Default 1:2 (-s2)\n"
			"\t-v VERBOSE    -\t Don't go into bacground\n"
			"\t-h HELP       -\t Display this help message\n"
		"\n", GLEM_VER_MAJ, GLEM_VER_MAJ
	);
}

void die_usage()
{
	glem_usage();
	exit(-1);
}

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

void glut_set_pixel(uint8_t *buf, int x, int y, int color)
{
	int i=0;
	for(i=0; i < 3; i++ ) {  //RGB Mirrors
		buf[(y*gl_width+x)*3+i] = color;
	}
}

void glut_set_scaled_pixel(uint8_t *buf, int x, int y, int color)
{
	int i, j, setVal;
	setVal = color ? 255 : 0;
	convert_local_to_glut(x, y);
	for (i=x; i<(x+scale_factor); i++) {
		for (j=y; j<(y+scale_factor); j++) {
			glut_set_pixel(buf, i, j, setVal);
		}
	}
}

void glut_make_buf(uint8_t *buf)
{
	int i, j, pix;
	for (i=0; i<glcd_width; i++) {
		for (j=0; j<glcd_height; j++) {
			pix = glcd_get_pixel(glcd_buf, i, j);
			glut_set_scaled_pixel(buf, i, (glcd_height-1)-j, pix);
		}
	}
}

void draw_rect_thick(uint8_t *buf, int x, int y, int w, int h, int tx, int ty, int color)
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
			glut_set_pixel(buf, i, y+t, color);
			glut_set_pixel(buf, i, y+h-1-t, color);
		}
	}
	for (i=y; i<y+h; i++) {
		/* Left and right sides */
		for (t=0; t<(tx); t++) {
			glut_set_pixel(buf, x+t, i, color);
			glut_set_pixel(buf, x+w-1-t, i, color);
		}
	}
}

void glut_redraw_screen()
{
	int x = window_origin_x-2;
	int y = window_origin_y-2;
	int w = gl_width-GL_PAD+4;
	int h = gl_height-GL_PAD+4;
	uint8_t *buf = calloc(gl_width * gl_height * 3, sizeof(uint8_t));
	if (buf == NULL)
		die("Out of memory at screen redraw (init)");
	draw_rect_thick(buf, x, y, w, h, 1, 1, 255);
	glut_make_buf(buf);
	glDrawPixels(gl_width, gl_height, GL_RGB, GL_UNSIGNED_BYTE, buf);
	glutSwapBuffers();
	glutPostRedisplay();
	free(buf);
}

void glut_reshape(int x, int y)
{
	glViewport(0, 0, (GLsizei) gl_width+100, (GLsizei) gl_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, gl_width, 0.0, gl_height, -1.f, 1.f);
}

void glut_timed_redraw(int t)
{
	glut_redraw_screen();
	glutTimerFunc(redisplay_interval, glut_timed_redraw, 0);
}

void glut_set_fps(int fps)
{
	redisplay_interval = 1000 / fps;
	glutTimerFunc(redisplay_interval, glut_timed_redraw, 0);
}

int glem_setpixel(struct glem_cmd_set_pixel *s)
{
	if (s->pix_x >= glcd_width || s->pix_y >= glcd_height ||
			s->pix_x < 0 || s->pix_y < 0)
		return -1;

	if (s->pix_color) {
		set_pix_rowmaj(s->pix_x, s->pix_y);
	} else {
		clear_pix_rowmaj(s->pix_x, s->pix_y);
	}
	return 0;
}

int glem_write_frame(struct glem_cmd_frame *s)
{
	if (s->res_x != glcd_width || s->res_y != glcd_height)
		return -1;

	if (s->buf_len != (glcd_width*glcd_height/8))
		return -1;

	memcpy(glcd_buf, s->buf, s->buf_len);
	return 0;
}

int glem_clear_screen()
{
	memset(glcd_buf, 0x00, glcd_width*glcd_height/8);
	return 0;
}

int glem_process_command(glem_command_t *cmd)
{
	int len, ret_val = -1;
	if (cmd == NULL)
		return ret_val;

	struct glem_cmd_frame * f;
	struct glem_cmd_set_pixel *s;

	switch(cmd->cmd_num) {
	case GLEM_CMD_SET_PIX:
		printf("glem: cmd: GLEM_CMD_SET_PIX\n");
		s = (struct glem_cmd_set_pixel *) cmd->data;
		if (sizeof(struct glem_cmd_set_pixel) != cmd->data_len) {
			break;
		}
		if (glem_setpixel(s) != 0) {
			break;
		}
		ret_val = 0;
		break;
	case GLEM_CMD_SEND_FRAME:
		printf("glem: cmd: GLEM_CMD_SEND_FRAME\n");
		f = (struct glem_cmd_frame *) cmd->data;
		len = sizeof(struct glem_cmd_frame) + f->buf_len;
		if (len != cmd->data_len) {
			printf("glem: data length mismatch!\n");
			break;
		}
		if (glem_write_frame(f) != 0) {
			break;
		}
		ret_val = 0;
		break;
	case GLEM_CMD_CLEAR_SCREEN:
		printf("glem: cmd: GLEM_CMD_CLEAR_SCREEN\n");
		glem_clear_screen();
		ret_val = 0;
		break;
	default:
		printf("glem: received unknown command!\n");
		break;
	}
	return ret_val;
}

void glem_socket_listener(void *args)
{
	int est_read_size = glcd_width*glcd_height/8 + 250; // +250 for meta data.
	int server_fd, client_fd;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;

	server_fd = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(31337);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	bind(server_fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	if(listen(server_fd, 5) != 0) {
		printf("listen on socket!");
	}

	while (1) {
		socklen_t addr_size = sizeof(serverStorage);
		client_fd = accept(server_fd, (struct sockaddr *) &serverStorage, &addr_size);
		uint8_t *tmp_buf = malloc(est_read_size);
		if (tmp_buf == NULL)
			die("Out of memory");

		int rec = read(client_fd, tmp_buf, est_read_size);
		if (rec > 0) {
			printf("glem: received %d bytes from client\n", rec);
			// Race condition: The consumer thread may have been
			// reading when we call memcpy here causing tear. But
			// we can live with it.
			glem_command_t *cmd = (glem_command_t *)tmp_buf;
			if (glem_process_command(cmd) != 0) {
				printf("glem: error processing command!!\n");
			}
		} else {
			// Read failed ??
			perror("glem: read failed");
		}
		free(tmp_buf);
		close(client_fd);
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

	umask(0); // Mark this process file creation mask to 0.

	if(setsid() < 0) { // Mark this child as session leader.
		die_error("Error going into the background");
	}

	// Close all 3 file desc to prevent any output to terminal.
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	return(0);
}

void glem_compute_dimensions()
{
	gl_width = glcd_width * scale_factor + GL_PAD;
	gl_height = glcd_height * scale_factor + GL_PAD;
	window_origin_x = (gl_width/2)-((gl_width-GL_PAD)/2);
	window_origin_y = (gl_height/2)-((gl_height-GL_PAD)/2);
}

void glem_parse_cla(int argc, char **argv)
{
	int opt, got_resolution=0, got_verbose=0;
	scale_factor = 2;
	while ((opt = getopt(argc, argv, "vhr:s:")) != -1) {
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
		case 'h':
			glem_usage();
			exit(0);
		default:// '?'
			printf("glem: invalid arguement\n");
			die_usage();
		}
	}
	if (!got_resolution) {		// Can't assume defaults here. Both these
		die_usage();		// are critical data. Hence mandated.
	}
	if (!got_verbose) {
		glem_background();
	}
}

int main(int argc, char **argv)
{
	glem_parse_cla(argc, argv);

	glem_compute_dimensions();

	//glem_init_server();
	pthread_t server_thread;
	pthread_create(&server_thread, NULL, (void *)glem_socket_listener, NULL);

	glcd_buf = calloc(glcd_width * glcd_height / 8, sizeof(uint8_t));
	if (glcd_buf == NULL)
		die("GLCD buf alloc failed");

	glutInitWindowSize(gl_width, gl_height);
	glutInit(&argc, argv);
	glutReshapeFunc(glut_reshape);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	char win_name[64];
	snprintf(win_name, 64, "glem v%d.%d (%dx%d-%d)",GLEM_VER_MAJ,
			GLEM_VER_MIN, glcd_width, glcd_height, scale_factor);
	win_name[63]=0; // paranoia: force terminate.

	int win_id = glutCreateWindow(win_name);
	glutSetWindow(win_id);
	glutDisplayFunc(glut_redraw_screen);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(255.0, 255.0, 255.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glClear(GL_POINT_BIT);
	glut_set_fps(5);
	glutMainLoop();	// This is blocking!
	return 0;
}

