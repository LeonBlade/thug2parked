#include <stdio.h>
#include <stdlib.h>

#include "t2cap.h"

uint16_t swap_uint16(uint16_t *ptr) 
{
    *ptr = (*ptr << 8) | (*ptr >> 8);
    return *ptr;
}

uint32_t swap_uint32(uint32_t *ptr)
{
    *ptr = ((*ptr << 8) & 0xFF00FF00) | ((*ptr >> 8) & 0xFF00FF); 
    return *ptr = (*ptr << 16) | (*ptr >> 16);
}

bool tested_endian = false;
bool big_endian = false;

size_t _fread(uint16_t *ptr, size_t size, size_t count, FILE *stream)
{
	size_t result;
	result = fread(ptr, size, count, stream);
	if (big_endian)
		swap_uint16((uint16_t*)ptr);
	return result;
}
size_t _fread(uint32_t *ptr, size_t size, size_t count, FILE *stream)
{
	size_t result;
	result = fread(ptr, size, count, stream);
	if (big_endian)
		swap_uint32((uint32_t*)ptr);
	return result;
}

bool _seek_cmp_read(FILE *stream, int ahead, uint32_t cmp)
{
	fpos_t pos; 
	fgetpos(stream, &pos);
	fseek(stream, ahead, SEEK_CUR);
	uint32_t b; 
	_fread(&b, 4, 1, stream);
	fsetpos(stream, &pos);
	return (b == cmp);
}

T2CAP_GROUND *load_cap(const char *filename)
{
	// get endian for _fread
	if (!tested_endian)
	{
		int num = 1;
		if (*(char*)&num == 1)
			big_endian = true;
		else
			big_endian = false;
		tested_endian = true;
	}

	// open file for read binary
	FILE *f = fopen(filename, "rb");
	if (f == NULL) 
	{
		fputs("Couldn't open file.", stderr);
		return NULL;
	}

	printf("Reading '%s'...\n", filename);

	// create t2cap file
	T2CAP_HEADER header;

	_fread(&header.checksum, 4, 1, f);
	_fread(&header.uchecksum, 4, 1, f);
	_fread(&header.len_park_name, 2, 1, f);
	_fread(&header._null1, 2, 1, f);
	_fread(&header.padding_offset, 2, 1, f);
	_fread(&header._null2, 1, 2, f);
	fread(&header.unknown1, 1, 1, f);
	_fread(&header.unknown2, 4, 1, f);

	_fread(&header.qb_num_edited_goals, 4, 1, f);
	fread(&header.num_edited_goals, 1, 1, f);
	
	_fread(&header.qb_maxplayers, 4, 1, f);
	fread(&header._max_players, 1, 1, f);
	fread(&header.unknown3, 1, 1, f);

	_fread(&header.qb_num_gaps, 4, 1, f);
	if (!_seek_cmp_read(f, 1, 0x35DCF3FF))
		fread(&header._num_gaps, 1, 1, f);
	else
		header._num_gaps = 0x00;
	fread(&header.unknown4, 1, 1, f);

	_fread(&header.qb_num_pieces, 4, 1, f);
	if (!_seek_cmp_read(f, 1, 0xF7188A68))
		fread(&header._num_pieces, 1, 1, f);
	else
		header._num_pieces = 0x00;
	fread(&header.unknown5, 1, 1, f);

	_fread(&header.qb_theme, 4, 1, f);
	if (!_seek_cmp_read(f, 1, 0x98ED724C))
		fread(&header.theme, 1, 1, f);
	else
		header.theme = 0x00;
	fread(&header.unknown5, 1, 1, f);

	_fread(&header.qb_tod_script, 4, 1, f);
	_fread(&header.tod, 4, 1, f);

	_fread(&header.qb_height, 2, 1, f);
	fread(&header._height, 1, 1, f);
	fread(&header.unknown7, 1, 1, f);

	_fread(&header.qb_length, 4, 1, f);
	fread(&header.length, 1, 1, f);
	fread(&header.unknown8, 1, 1, f);

	_fread(&header.qb_filename, 4, 1, f);

	int c;
	int n = 0;
	do { c = fgetc(f); n++; } while (c != '\0');
	header.filename = (char*)malloc(sizeof(char)*n);
	fseek(f, -n, SEEK_CUR);
	fread(header.filename, n, 1, f);

	fread(&header.unknown9, 1, 1, f);

	fread(&header.unknown10, 1, 1, f);
	_fread(&header.qb_park_editor_map, 4, 1, f);
	fread(&header.unknown11, 1, 1, f);

	_fread(&header.unknown12, 2, 1, f);
	_fread(&header.unknown13, 4, 1, f);
	_fread(&header.unknown14, 4, 1, f);

	fread(&header.theme, 1, 1, f);
	fread(&header.unknown15, 3, 1, f);
	_fread(&header.position, 2, 1, f);
	fread(&header.width, 1, 1, f);
	fread(&header.height, 1, 1, f);
	_fread(&header.num_park_pieces, 2, 1, f);
	_fread(&header.num_gaps, 2, 1, f);
	fread(&header.max_players, 1, 1, f);
	fread(&header.unknown16, 7, 1, f);
	
	n = 0;
	do { c = fgetc(f); n++; } while (c != '\0');
	header.park_name = (char*)malloc(sizeof(char)*n);
	fseek(f, -n, SEEK_CUR);
	fread(header.park_name, n, 1, f);
	fread(&header._null3, 1, 1, f);

	printf("Header information read.\n");
	printf("Seeking ahead to ground data at 0x%02X...\n", 0xE8 + (n - 1));
	
	fseek(f, 0xE8 + (n - 1), SEEK_SET);
	
	T2CAP_GROUND *ground = (T2CAP_GROUND*)malloc(0xCB0);
	/*
	for (int i = 0; i < 56; i++)
	{
		fpos_t pos;
		fgetpos(f, &pos);
		printf("@ 0x%02X\n", (int)pos);
		for (int j = 0; j < 0x39; j++)
		{
			int8_t c;
			fread(&c, 1, 1, f);
			printf("%02X ", (uint8_t)c);
			ground[i].column[j] = c;
		}
		fseek(f, 1, SEEK_CUR);
		printf("\n\n");
	}
	*/
	fread(ground, sizeof(T2CAP_GROUND), 56, f);

	//T2CAP_OBJECT *objects = (T2CAP_OBJECT*)malloc(sizeof(T2CAP_OBJECT)*);

	return ground;
}
