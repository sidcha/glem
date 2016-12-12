#ifndef __OBJECTS_H_
#define __OBJECTS_H_

#include <stdint.h>

#define OBJ_ROW_MAJOR 0x00
#define OBJ_COL_MAJOR 0x01

typedef struct {
	int char_width;
	int offset;
}font_descriptor_t;

typedef struct {
	const int char_space;
	const int char_height;
	const int start_char;
	const int end_char;
	const font_descriptor_t *desc;
	const int flags;
	const uint8_t *bitmap;
}font_t;

typedef struct {
	const int width;
	const int height;
	const int flags;
	const uint8_t *bitmap;
}symbol_t;

#endif
