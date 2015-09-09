#pragma once
#include "B_Tile.h"
#include <vector>
/*
This class is the main class of the backend of the project.
A board is composed of 121 different tiles all placed in a vector. This class
handles all the algorithm for movement, attacking and such.
@author: Kevin Tanguay
*/
class B_Board
{
	public:
		B_Board();
		~B_Board();
		int getSize();
		void generateTiles();
		std::vector<B_Tile*> tiles;
		std::string getWhitePieces();
		std::string getBlackPieces();
		bool isTileEmpty(B_Tile* tile);
		bool doesTileHavePiece(B_Tile* tile);
		bool tileIsACastle(B_Tile* tile);
		bool tileIsThrone(B_Tile* tile);
		std::string checkMoveDirection(int intinitialPosition, int vectorPosition);
		void checkAdjacentEnemy(int vectorPosition, std::string moveDirection);
		bool isThereAnEnemy(int vectorPosition, std::string playerColor, bool secondLook);
		void movePiece(B_Tile* initialTile, B_Tile* tile);
		void destroyEnemy(B_Tile* enemyToRemove);
	private:
		int whitePieces;
		int blackPieces;
		bool findIntInArray(const int intArray[], int size, int number);
		bool attackTheKing(int vectorPosition);
		int size;
		bool secondEnemyCheck;	
};



