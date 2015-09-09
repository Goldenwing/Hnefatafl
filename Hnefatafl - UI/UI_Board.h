#pragma once
#include "../TGUI/include/TGUI/TGUI.hpp"
#include "B_Board.h"
#include "Constants.h"
#include <vector>

/*
This class is the main class for everything interface. The visuals, the board,
the grid, the different buttons, everything is handled within this class. 
The class interrogates the backend grid to update itself accordingly to each action.
@author : Kevin Tanguay
*/

class UI_Board
{
	public:
		UI_Board();
		~UI_Board();
		std::vector<std::string> grid;

	private:
		void generateBoard();
		void runGame();
		void createWindow(std::string windowType, int x, int y);
		void specialKingMovement(int start, int destination);
		B_Board* gameBoard;
		bool findIntInArray(const int intArray[], int size, int number);
		void checkMovementRange(int start);
		tgui::Picture::Ptr picArray[121];
		tgui::Label::Ptr whiteDeaths;
		tgui::Label::Ptr blackDeaths;
		std::vector<int> pieceMovementRange;
		void emptyPreviousMovements();
		void movePiece(int pieceToMove, int destination);
		int pieceInMovement;
		void updateBoard();
		void resetGame();
		tgui::Label::Ptr whoseTurnLbl;
		std::string whoseTurn;
		bool isKingDead = false;
		bool didKingWin = false;
		bool gameClosed = false;
		void victory(std::string winner, int numberOfTimes);
		
};

