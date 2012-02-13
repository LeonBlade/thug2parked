#ifndef _T2_CAP_H_
#define _T2_CAP_H_

#include <stdint.h>

struct T2CAP_HEADER
{
	uint32_t checksum;

	uint32_t uchecksum; 
	uint16_t len_park_name;
	uint16_t _null1;
	uint16_t padding_offset;
	uint16_t _null2;
	uint8_t unknown1;
	uint32_t unknown2; // 00 00 00 12

	uint32_t qb_num_edited_goals; // 6F 60 EC E1
	uint8_t num_edited_goals; // unused ?

	uint32_t qb_maxplayers; // 53 9B E3 B7
	uint8_t _max_players;
	uint8_t unknown3; // 12 ?

	uint32_t qb_num_gaps; // D0 1E 12 E6
	uint8_t _num_gaps;
	uint8_t unknown4; // 12 ?

	uint32_t qb_num_pieces; // 35 DC F3 FF
	uint8_t _num_pieces;
	uint8_t unknown5; // 12 ?

	uint32_t qb_theme; // F7 18 8A 68
	uint8_t _theme;
	uint8_t unknown6; // 0D ?

	uint32_t qb_tod_script; // 98 ED 72 4C
	uint32_t tod; // 20 FF A1 1C (default)

	uint16_t qb_height; // 90 EF
	uint8_t _height;
	uint8_t unknown7; // 10 ?

	uint32_t qb_length; // 4D 61 82 FE
	uint8_t length;
	uint8_t unknown8; // 03 ?

	uint32_t qb_filename; // 9A 16 F4 C3
	char *filename;
	uint8_t unknown9; // 00

	uint8_t unknown10; // 0C
	uint32_t qb_park_editor_map; // 89 52 7C 33
	uint8_t unknown11; // 01

	uint16_t unknown12; // A6 0C
	uint32_t unknown13;
	uint32_t unknown14; // ?? 0D 24 4E (?? = 80 + (cap pieces * 8))

	uint8_t theme;
	uint8_t unknown15[3]; // 00 00 00
	uint16_t position; // XX YY ?
	uint8_t width;
	uint8_t height;
	uint16_t num_park_pieces;
	uint16_t num_gaps;
	uint8_t max_players;
	uint8_t unknown16[7]; // 00 00 00 00 00 00 00
	char *park_name;
	uint8_t _null3;
};

struct T2CAP_GROUND
{
	int8_t column[0x39];
	uint8_t _null;
};

struct T2CAP_OBJECT
{
	uint8_t object;
	uint8_t x;
	uint8_t y;
	uint8_t _null; // 00
	uint8_t height_dir;
};

struct T2CAP_GAP
{
	uint8_t x1;
	uint8_t x2;
	uint8_t height1;
	uint8_t height2;
	uint8_t y1;
	uint8_t y2;
	uint8_t length1;
	uint8_t length2;
	uint8_t rotation2;
	uint8_t rotation1;
	uint8_t _null1;
	char name[20];
	uint16_t score;
	uint8_t type[3]; // 01 00 00 (All) FD 01 00 (Only Air) F9 01 00 (Only Rail) BD 01 40 (Only Manual)
	uint8_t _null2;
};

struct T2CAP_RAIL_DATA
{
	uint16_t unknown1; // 00 0C
	uint32_t qb_points; // D6 71 45 D8
	uint8_t unknown2; // 0A
	uint16_t unknown3; // 02 00

	uint8_t unknown4; // 06
	uint32_t qb_pos; // 53 19 26 7F

	uint32_t x;
	uint32_t height; 
	uint32_t y;

	uint8_t unknown5; // 0D ?
	uint32_t _null1;
	uint32_t qb_haspost; // 72 D7 DE DC
	uint8_t _null2; // 00

	// Special height
	//
	// 00 00 00 00 (Ground) 
	// 00 00 A0 41 (Minimum base height)
	// 00 00 E8 00 (One block up) 
	// 00 00 88 42 (One block up + min base height)
};

struct T2CAP_RAIL
{
	uint8_t unknown1; // 0A
	uint32_t qb_park_editor_goals; // 5E 82 EB D8
	uint8_t unknown2; // 0C
	uint32_t qb_goals; // D0 E1 DB 38
	uint32_t unknown3; // 00 00 00 00
	uint8_t unknown4; // 0C
	uint32_t qb_created_rails; // A6 50 45 24
	uint8_t unknown5;

	uint8_t num_of_rails;

	T2CAP_RAIL_DATA *rail_data;

	uint16_t unknown6; // 00 01
	uint32_t qb_maxplayers; // 53 9B E3 B7
	uint16_t num_of_players;
};

struct T2CAP_FILE
{
	T2CAP_HEADER header;
	T2CAP_GROUND *ground;
	uint8_t _null[57];
	T2CAP_OBJECT *objects;
	T2CAP_GAP *gaps;
	T2CAP_RAIL rail;
};

T2CAP_GROUND *load_cap(const char *filename);

#endif