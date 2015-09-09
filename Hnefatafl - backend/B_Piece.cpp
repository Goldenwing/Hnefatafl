#include "B_Piece.h"
/*
This page handles all the functions and methods related to
the B_Piece class.
*/

/*Default constructor... never used.*/
B_Piece::B_Piece()
{

}

/*Constructor that takes the color of the piece. An empty tile will have
a piece with a blank color and the bool "initialized" at false.
@param color : the color of the piece. Either Black, White, or empty
*/
B_Piece::B_Piece(std::string color)
{
	this->color = color;
	this->initialized = true;
	if (color == "")
	{
		this->initialized = false;
	}
}

/*Deconstructor*/
B_Piece::~B_Piece()
{

}

/* 
Returns the color of the piece
@return color: the color of the piece*/
std::string B_Piece::getColor()
{
	return this->color;
}

/*
Checks if the piece on a tile is an actual useable game piece. 
@return initialized: true if a piece, false if empty
*/
bool B_Piece::ifExists()
{
	return this->initialized;
}