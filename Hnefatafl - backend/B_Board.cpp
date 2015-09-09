#include "B_Board.h"
#include "Constants.h"
#include "B_Piece.h"
#include "B_Tile.h"
#include <ostream>
#include <sstream>
/*
This page includes all the functions and methods for the B_Board class.
*/

/*
Constructor. It initializes the board for a new game to start, and fills the
vector of tile pointers.
*/
B_Board::B_Board()
{
	this->blackPieces = Constants::NO_BLACK_PIECES;
	this->whitePieces = Constants::NO_WHITE_PIECES;
	this->size = Constants::BOARD_SIZE;
	this->secondEnemyCheck = false;
	generateTiles();
}

/*
Deconstructor. Since the vector is a vector of pointers, we must iterate
through every entry and delete the reserved space, then we can clear the
vector.
*/
B_Board::~B_Board()
{
	for (std::vector< B_Tile* >::iterator it = this->tiles.begin(); it != this->tiles.end(); ++it)
	{
		delete (*it);
	}
	this->tiles.clear();
}

/*
Returns the size of the board.
@return size: the size of the board.
*/
int B_Board::getSize()
{
	return this->size;
}

/*
Returns the number of white pieces still alive.
@return whitePieces: the number of white pieces alive.
*/
std::string B_Board::getWhitePieces()
{
	return std::to_string(this->whitePieces);
}

/*
Returns the number of black pieces still alive.
@return blackPieces: the number of black pieces alive.
*/
std::string B_Board::getBlackPieces()
{
	return std::to_string(this->blackPieces);
}

/*
The main function that creates each tile and fills the vector. Depending
on the counter that iterates 121 times, the new tile will either be a castle, a Throne, or a blank.
Also determines which piece (black or white) goes onto the tile if it's at a correct position.
*/
void B_Board::generateTiles()
{
	int castleCounter = 0, whiteCounter = 0, blackCounter = 0, pieceCounter = 0;

	while (pieceCounter < Constants::NO_TILES)
	{
		B_Tile* newTile = new B_Tile("BLANK", pieceCounter);

		if (findIntInArray(Constants::CASTLE_POSITIONS, Constants::NO_CASTLES, pieceCounter))
		{
			castleCounter++;
			newTile->setType("castle");
		}

		//the first two conditions is simply to avoid having to enter the findIntInArray method when unneccessary,
		//thus saving time. I use it in the other "ifs" as well.
		else if ( pieceCounter >= 38 && whiteCounter < Constants::NO_WHITE_PIECES &&
			findIntInArray(Constants::WHITE_POSITIONS, Constants::NO_WHITE_PIECES, pieceCounter))
		{
			whiteCounter++;
			B_Piece newPiece("white");
			newTile->setPiece(newPiece);
		}

		else if (blackCounter < Constants::NO_BLACK_PIECES &&
			findIntInArray(Constants::BLACK_POSITIONS, Constants::NO_BLACK_PIECES, pieceCounter))
		{
			blackCounter++;
			B_Piece newPiece("black");
			newTile->setPiece(newPiece);
		}

		else if (pieceCounter == Constants::THRONE_POSITON)
		{
			B_Piece newPiece("whiteK");
			newTile->setType("throne");
			newTile->setPiece(newPiece);
		}

		this->tiles.push_back(newTile);

		pieceCounter++;
	}

}

/*
A simple method that takes a number and looks inside an array to see if
the number is contained within the array.
@param intArray[]: the array of numbers we want to look into;
@param size: the size of the array;
@param number: the number we're looking for.
@return bool: false if not found, true if found.
*/
bool B_Board::findIntInArray(const int intArray[], int size, int number)
{
	for (int i = 0; i < size; i++)
	{
		if (intArray[i] == number)
		{
			return true;
		}
	}
	return false;
}


bool B_Board::isTileEmpty(B_Tile* tile)
{
	if (!doesTileHavePiece(tile) && !tileIsACastle(tile) && !tileIsThrone(tile))
	{
		return true;
	}
	return false;
}

/*
Checks if the tile has a playable piece on it.
@return bool: true if piece found, false if not.
*/
bool B_Board::doesTileHavePiece(B_Tile* tile)
{
	return tile->getPiece().ifExists();
}

/*
The main method that triggers enemy detection, and by doing so, enemy elimination. A piece can
only move in a straight line, horizontally or vertically, as long as there's no obstacles.
@param initialTile: The tile on which the piece is currently at;
@param tile: the destination of the movement.
*/
void B_Board::movePiece(B_Tile* initialTile, B_Tile* tile)
{
	int initialVectorPosition = (initialTile->getX() * Constants::BOARD_SIZE) + initialTile->getY();
	int destinationVectorPosition = (tile->getX() * Constants::BOARD_SIZE) + tile->getY();

	this->tiles.at(destinationVectorPosition)->setPiece(this->tiles.at(initialVectorPosition)->getPiece());
	this->tiles.at(initialVectorPosition)->setPiece(B_Piece(""));
	
	std::string moveDirection = checkMoveDirection(initialVectorPosition, destinationVectorPosition);
	checkAdjacentEnemy(destinationVectorPosition, moveDirection);
}

/*
Checks from which direction the moved piece comes from. This method is solely used to avoid
having to check behind the piece when detecting enemies, thus saving a calculation.
@param initialPosition: the vector position of the piece, before movement;
@param vectorPosition: the vector position of the piece, after movement.
*/
std::string B_Board::checkMoveDirection(int initialPosition, int vectorPosition)
{
	//coming from right  
	if ((vectorPosition - initialPosition) % 11 == 0 && (vectorPosition - initialPosition) > 0)
	{
		return "right";
	}

	//coming from left 
	else if ((initialPosition - vectorPosition) % 11 == 0 && (initialPosition - vectorPosition) > 0)
	{
		return "left";
	}

	//coming from up 
	else if ((initialPosition - vectorPosition) < 11 && (initialPosition - vectorPosition) > 0)
	{
		return "up";
	}

	else
	{
		return "down";
	}
}

/*
The second main method. After movement, it looks in every direction to see if there is an enemy
either up, down, left or right of the moved piece. If there is an enemy, it will check the tile after the
inspected tile to see if the enemy is killed or not. An enemy is dead when two of the same colored piece
surround an opposing color piece.
@param vectorPosition: the vector position of the piece;
@moveDirection: the direction in which the piece moved from.
*/
void B_Board::checkAdjacentEnemy(int vectorPosition, std::string moveDirection)
{
	std::string playerColor = this->tiles.at(vectorPosition)->getPiece().getColor();

	//If a tile is adjacent to a top or bottom extremity (or one tile under), it will not 
	//look in that direction.
	if (!findIntInArray(Constants::TOP_BORDER_POSITIONS, 9, vectorPosition) &&
		!findIntInArray(Constants::TOP_BORDER_POSITIONS, 9, (vectorPosition - 1)))
	{
		if (isThereAnEnemy(vectorPosition - 1, playerColor, false) && moveDirection != "down")
		{
			if (isThereAnEnemy(vectorPosition - 2, playerColor, true))
			{
				destroyEnemy(this->tiles.at(vectorPosition - 1));
			}
		}
	}

	if (!findIntInArray(Constants::BOTTOM_BORDER_POSITIONS, 9, vectorPosition) &&
		!findIntInArray(Constants::BOTTOM_BORDER_POSITIONS, 9, (vectorPosition + 1)))
	{
		if (isThereAnEnemy(vectorPosition + 1, playerColor, false) && moveDirection != "up")
		{
			if (isThereAnEnemy(vectorPosition + 2, playerColor, true))
			{
				destroyEnemy(this->tiles.at(vectorPosition + 1));
			}
		}
	}

	if (isThereAnEnemy(vectorPosition - 11, playerColor, false) && moveDirection != "right")
	{
		if (isThereAnEnemy(vectorPosition - 22, playerColor, true))
		{
			destroyEnemy(this->tiles.at(vectorPosition - 11));
		}
	}

	if (isThereAnEnemy(vectorPosition + 11, playerColor, false) && moveDirection != "left")
	{
		if (isThereAnEnemy(vectorPosition + 22, playerColor, true))
		{
			destroyEnemy(this->tiles.at(vectorPosition + 11));
		}
	}

}

/*
Inspects the tile at the vector position to see if it is an enemy. Depending on if it's the second look,
it will have different behaviors. There are many different results possible, what with castles, thrones and 
such. This method inspects it all depending on if it's the first or second look.
@param vectorPosition: the vector position of the tile;
@param playerColor: the color of the piece that was moved... used to determine the enemy color;
@param secondLook: True if it's the second enemy inspection, false if the first.
@returns bool: true if enemy found, false if not.
*/
bool B_Board::isThereAnEnemy(int vectorPosition, std::string playerColor, bool secondLook)
{
	if (vectorPosition < 0 || vectorPosition > 120)
	{
		return false;
	}

	if (!secondLook)
	{
		if (doesTileHavePiece(this->tiles.at(vectorPosition)))
		{
			if (this->tiles.at(vectorPosition)->getPiece().getColor() == "whiteK")
			{
				if(playerColor == "black")
					return attackTheKing(vectorPosition);
			}
			else
			{
				if (playerColor == "whiteK" && this->tiles.at(vectorPosition)->getPiece().getColor() == "white")
				{
					return false;
				}
				if (this->tiles.at(vectorPosition)->getPiece().getColor() != playerColor)
				{
					return true;
				}
			}
		}
		return false;
	}
	else
	{
		if (doesTileHavePiece(this->tiles.at(vectorPosition)))
		{
			if (this->tiles.at(vectorPosition)->getPiece().getColor() == "whiteK")
			{
				if (playerColor == "white")
				{
					return true;
				}
			}
			else
			{
				if (this->tiles.at(vectorPosition)->getPiece().getColor() == playerColor)
				{
					return true;
				}
				else
				{
					if (playerColor == "whiteK" && this->tiles.at(vectorPosition)->getPiece().getColor() == "white")
					{
						return true;
					}
					if (playerColor == "white" && this->tiles.at(vectorPosition)->getPiece().getColor() == "whiteK")
					{
						return true;
					}
				}
			}
			return false;
		}

		if (tileIsACastle(this->tiles.at(vectorPosition)))
		{
			return true;
		}

		if (tileIsThrone(this->tiles.at(vectorPosition)))
		{
			if (playerColor == "white" || playerColor == "whiteK")
			{
				return true;
			}
			else if (playerColor == "black" && !doesTileHavePiece(this->tiles.at(vectorPosition)))
			{
				return true;
			}
			else
				return false;
		}
		return false;
	}
}

/*
Returns confirmation that the tile is or is not a castle tile.
@return bool: true if the tile is a castle, false if not.
*/
bool B_Board::tileIsACastle(B_Tile* tile)
{
	return tile->getType() == "castle";
}

/*
Returns confirmation that the tile is or is not the throne tile.
@return bool: true if the tile is the throne, false if not.
*/
bool B_Board::tileIsThrone(B_Tile* tile)
{
	return tile->getType() == "throne";
}

/*
Method that eliminates the piece on the tile. The tile is inserted a blank piece, and
the number of pieces of the deceased color goes down by one.
@param enemyToRemove: the tile upon which the dead piece resides.
*/
void B_Board::destroyEnemy(B_Tile* enemyToRemove)
{
	if (enemyToRemove->getPiece().getColor() == "black")
	{
		this->blackPieces--;
	}
	else
	{
		this->whitePieces--;
	}
	enemyToRemove->setPiece(B_Piece(""));
}

/*
Special attacking method specifically used for the King. Since he has a unique condition
to die, this method is necessary. For a king to die, it must be completely
surrounded by black tiles, walls do not count.
@param vectorPosition: the vector position of the King
@return bool: true if the king is dead, false if not.
*/
bool B_Board::attackTheKing(int vectorPosition)
{
	if (!findIntInArray(Constants::BOTTOM_BORDER_POSITIONS, 9, vectorPosition + 1) &&
		doesTileHavePiece(this->tiles.at(vectorPosition + 1)) &&
		this->tiles.at(vectorPosition + 1)->getPiece().getColor() == "black")
	{
		if (!findIntInArray(Constants::TOP_BORDER_POSITIONS, 9, vectorPosition - 1) &&
			doesTileHavePiece(this->tiles.at(vectorPosition - 1)) &&
			this->tiles.at(vectorPosition - 1)->getPiece().getColor() == "black")
		{
			if (vectorPosition + 11 < 120 &&
				doesTileHavePiece(this->tiles.at(vectorPosition + 11)) &&
				this->tiles.at(vectorPosition + 11)->getPiece().getColor() == "black")
			{
				if (vectorPosition - 11 > 0 &&
					doesTileHavePiece(this->tiles.at(vectorPosition - 11)) &&
					this->tiles.at(vectorPosition - 11)->getPiece().getColor() == "black")
				{
					return true;
				}
			}
		}
	}
	return false;
}