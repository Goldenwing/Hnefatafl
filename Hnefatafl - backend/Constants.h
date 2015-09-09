#pragma once
/*
This namespace contains all the constants that are used in the program. They
are all placed in a same page for ease of access. 
@author: Kevin Tanguay
*/
namespace Constants
{
	const int BOARD_SIZE = 11;
	const int NO_TILES = 121;
	const int NO_BLACK_PIECES = 24;
	const int NO_WHITE_PIECES = 12;
	const int NO_CASTLES = 4;

	const int CASTLE_POSITIONS[NO_CASTLES] =
	{ 0, 10, 110, 120 };

	const int BLACK_POSITIONS[NO_BLACK_PIECES] =
	{ 3, 4, 5, 6, 7, 16, 33, 43, 44, 54, 55, 56, 64,
	65, 66, 76, 77, 87, 104, 113, 114, 115, 116, 117 };

	const int WHITE_POSITIONS[NO_WHITE_PIECES] =
	{ 38, 48, 49, 50, 58, 59, 61, 62, 70, 71, 72, 82 };

	const int TOP_BORDER_POSITIONS[9] =
	{ 11, 22, 33, 44, 55, 66, 77, 88, 99 };

	const int BOTTOM_BORDER_POSITIONS[9] =
	{ 21, 32, 43, 54, 65, 76, 87, 98, 109 };

	const int THRONE_POSITON = 60;
};


