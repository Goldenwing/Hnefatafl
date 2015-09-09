#pragma once
#include <string>
/* 
This class handles everything related to each individual piece that 
is placed on the board. Each piece has a color, and each piece is inserted in a Tile. 
@author: Kevin Tanguay
*/
class B_Piece
{
	public:
		B_Piece();
		B_Piece(std::string color);
		~B_Piece();
		std::string getColor();
		bool ifExists();

	private:
		std::string color;
		bool initialized;
		
};

