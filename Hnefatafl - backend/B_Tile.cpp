#include "B_Tile.h"
#include <exception>
/*
This page handles all the methods and functions of the B_Tile class.
*/

/*Constructor. It takes the type and the tile number to assign a position as if
it were a two-dimensional array. The way the numbers go is vertically. 0-10 is the first column,
11-22 is the second, so on and so forth.
@param type: The type of the tile, either a castle, a throne, or blank.
@param tileNo : 121 in total. According to the number, it is assigned an X and a Y.
*/
B_Tile::B_Tile(std::string type, unsigned int tileNo)
{
	this->type = type;
	this->pieceOnTile = B_Piece("");

	if (tileNo < 11)
	{
		this->x = 0;
		this->y = tileNo;
	}
	else if (tileNo > 10 && tileNo < 22)
	{
		this->x = 1;
		this->y = (tileNo - 11);
	}
	else if (tileNo > 21 && tileNo < 33)
	{
		this->x = 2;
		this->y = (tileNo - 22);
	}
	else if (tileNo > 32 && tileNo < 44)
	{
		this->x = 3;
		this->y = (tileNo - 33);
	}
	else if (tileNo > 43 && tileNo < 55)
	{
		this->x = 4;
		this->y = (tileNo - 44);
	}
	else if (tileNo > 54 && tileNo < 66)
	{
		this->x = 5;
		this->y = (tileNo - 55);
	}
	else if (tileNo > 65 && tileNo < 77)
	{
		this->x = 6;
		this->y = (tileNo - 66);
	}
	else if (tileNo > 76 && tileNo < 88)
	{
		this->x = 7;
		this->y = (tileNo - 77);
	}
	else if (tileNo > 87 && tileNo < 99)
	{
		this->x = 8;
		this->y = (tileNo - 88);
	}
	else if (tileNo > 98 && tileNo < 110)
	{
		this->x = 9;
		this->y = (tileNo - 99);
	}
	else if (tileNo > 109 && tileNo < 121)
	{
		this->x = 10;
		this->y = (tileNo - 110);
	}
}

/*Deconstructor*/
B_Tile::~B_Tile()
{
}

/*
Returns the type of the tile.
@return type: can be blank, a castle, or the throne.
*/
std::string B_Tile::getType()
{
	return this->type;
}

/*
Returns the piece that is placed upon this tile.
@return pieceOnTile: The piece currently assigned to the tile.
*/
B_Piece B_Tile::getPiece()
{
	return this->pieceOnTile;
}

/*
Sets the type of the tile manually.
@param type: either blank, Castle, or throne.
*/
void B_Tile::setType(std::string type)
{
	this->type = type;
}

/*
Sets the piece that goes on the tile
@param piece: The piece that is placed onto the tile.
*/
void B_Tile::setPiece(B_Piece piece)
{
	this->pieceOnTile = piece;
}

/*
Returns the X position of the tile
@return x: the column position of the tile.
*/
int B_Tile::getX()
{
	return this->x;
}

/* 
Returns the Y position of the tile
@returns y: the row position of the tile.
*/
int B_Tile::getY()
{
	return this->y;
}