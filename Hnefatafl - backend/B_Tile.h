#pragma once
#include "B_Piece.h"
#include <string>
/*
This class handles every individual tile that composes the board. There are 121
tiles in total, each has a specific type, a specific position (a x and a y)
and a certain piece on it. The tiles are placed in a vector
@author: Kevin Tanguay
*/
class B_Tile
{
	public:
		B_Tile(std::string type, unsigned int tileNo);
		~B_Tile();
		std::string getType();
		B_Piece getPiece();
		void setType(std::string type);
		void setPiece(B_Piece piece);
		int getX();
		int getY();

	private:
		std::string type;
		unsigned int x;
		unsigned int y;
		B_Piece pieceOnTile;

};

