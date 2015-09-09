#include "UI_Board.h"
#include <Windows.h>

/*
This page includes all the functions and methods for the UI_Board class.
*/

/*
Constructor. This class will create the visual board after
having created the backend board. After having done that, it
launches the game, with the black pieces always starting.
*/
UI_Board::UI_Board()
{
	this->grid = std::vector<std::string>();
	this->pieceInMovement = 0;
	this->whoseTurn = "black";
	this->gameBoard = new B_Board();

	generateBoard();
	runGame();
}

/*
Deconstructor. It deletes the backend game, and clears
both vectors used. Although not pointers or vector of pointers, it's just
to finish clean.
*/
UI_Board::~UI_Board()
{
	delete this->gameBoard;
	this->grid.clear();
	this->pieceMovementRange.clear();
}

/*
This method will interrogate the backend board to
write down the necessary information to make the board. 
It recreates, in the grid variable,
the board layout with strings.
*/
void UI_Board::generateBoard()
{
	for (int i = 0; i < Constants::NO_TILES; i++)
	{
		if (this->gameBoard->doesTileHavePiece(this->gameBoard->tiles.at(i)))
		{
			if (this->gameBoard->tiles.at(i)->getPiece().getColor() == "white")
			{
				this->grid.push_back("white");
			}
			else if (this->gameBoard->tiles.at(i)->getPiece().getColor() == "black")
			{
				this->grid.push_back("black");
			}
			else if (this->gameBoard->tiles.at(i)->getPiece().getColor() == "whiteK")
			{
				this->grid.push_back("whiteK");
			}
		}
		else
		{
			if (this->gameBoard->tileIsACastle(this->gameBoard->tiles.at(i)))
			{
				this->grid.push_back("castle");
			}
			else
			{
				this->grid.push_back("BLANK");
			}
		}
	}
}

/*
The main artery of the class. This method creates the window and all of its components.
It will then launch a continuous loop to check for any user actions or board updates, 
and then react accordingly.
*/
void UI_Board::runGame()
{
	sf::RenderWindow window(sf::VideoMode(1000, 700), "Hnefatafl");
	tgui::Gui gui = tgui::Gui(window);
	gui.setGlobalFont("../TGUI/fonts/DejaVuSans.ttf");

	tgui::Picture::Ptr backgroundImage(gui);
	backgroundImage->load("../imgs/background.jpg");
	backgroundImage->setSize(1000, 700);

	tgui::Picture::Ptr resetBtn(gui);
	resetBtn->load("../imgs/resetBtn.jpg");
	resetBtn->setSize(200, 100);
	resetBtn->setPosition(100, 375);
	resetBtn->bindCallback(tgui::Button::LeftMouseClicked);
	resetBtn->setCallbackId(122);

	tgui::Picture::Ptr quitBtn(gui);
	quitBtn->load("../imgs/exitBtn.jpg");
	quitBtn->setSize(200, 100);
	quitBtn->setPosition(100, 525);
	quitBtn->bindCallback(tgui::Button::LeftMouseClicked);
	quitBtn->setCallbackId(123);

	tgui::Picture::Ptr ruleBtn(gui);
	ruleBtn->load("../imgs/ruleBtn.jpg");
	ruleBtn->setSize(200, 100);
	ruleBtn->setPosition(720, 5);
	ruleBtn->bindCallback(tgui::Button::LeftMouseClicked);
	ruleBtn->setCallbackId(124);

	this->whiteDeaths = tgui::Label::Ptr(gui);
	this->whiteDeaths->load("../TGUI/widgets/Black.conf");
	this->whiteDeaths->setText("White guards alive: 12");
	this->whiteDeaths->setTextSize(25);
	this->whiteDeaths->setSize(300, 40);
	this->whiteDeaths->setPosition(75, 200);

	this->whoseTurnLbl = tgui::Label::Ptr(gui);
	this->whoseTurnLbl->load("../TGUI/widgets/Black.conf");
	this->whoseTurnLbl->setText("Turn: " + this->whoseTurn);
	this->whoseTurnLbl->setTextSize(25);
	this->whoseTurnLbl->setSize(250, 40);
	this->whoseTurnLbl->setPosition(75, 250);

	this->blackDeaths = tgui::Label::Ptr(gui);
	this->blackDeaths->load("../TGUI/widgets/Black.conf");
	this->blackDeaths->setText("Black guards alive: 24");
	this->blackDeaths->setTextSize(25);
	this->blackDeaths->setSize(300, 40);
	this->blackDeaths->setPosition(75, 150);

	int x = 0, y = 0;

	for (int i = 0; i < Constants::NO_TILES; i++)
	{
		tgui::Picture::Ptr tile(gui);
		tile->load("../imgs/HNEF_TILE_" + this->grid.at(i) + ".gif");
		tile->setPosition((50 * x) + 400, ((50 * y) + 110));
		tile->bindCallback(tgui::Button::LeftMouseClicked);
		tile->setCallbackId(i);
		picArray[i] = tile;

		y++;
		if (y == 11)
		{
			x++;
			y = 0;
		}
	}
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			// Pass the event to all the widgets (if there would be widgets)
			gui.handleEvent(event);
		}

		tgui::Callback callback;
		while (gui.pollCallback(callback))
		{
			if (callback.id < 122)
			{
				if (this->grid.at(callback.id) == this->whoseTurn || 
					(this->grid.at(callback.id) == "whiteK" && this->whoseTurn == "white"))
				{
					this->pieceInMovement = callback.id;

					emptyPreviousMovements();
					checkMovementRange(callback.id);
				}
				else
				{
					if (std::find(this->pieceMovementRange.begin(), this->pieceMovementRange.end(), callback.id) != this->pieceMovementRange.end())
					{
						if (this->whoseTurn == "white")
						{
							this->whoseTurn = "black";
						}
						else
							this->whoseTurn = "white";

						movePiece(this->pieceInMovement, callback.id);
					}
					else
					{
						emptyPreviousMovements();
					}
				}
			}

			if (callback.id == 122)
			{
				createWindow("reset", 400, 200);
			}
			if (callback.id == 123)
			{
				createWindow("quit", 400, 200);
			}
			if (callback.id == 124)
			{
				createWindow("rules", 952, 369);
			}
		}

		if (isKingDead == true)
		{
			for (int i = 0; i < 5; i++)
			{
				victory("BLACK", i);
				window.clear();

				gui.draw();

				window.display();
			}
		
			this->isKingDead = false;
		}
		else if (didKingWin == true)
		{
			for (int i = 0; i < 5; i++)
			{
				victory("WHITE", i);
				window.clear();

				gui.draw();

				window.display();
			}
			
			this->didKingWin = false;
		}

		if (this->gameClosed)
			window.close();

		window.clear();

		gui.draw();

		window.display();
	}
}

/*
This method handles the extra windows that are created when the user
clicks on the different buttons. The windows being Rules, QuitGame, or Reset.
@param windowType : the window that is being opened, either rules, quit, or reset;
@param x : the length of the window size;
@param y : the width of the window size.
*/
void UI_Board::createWindow(std::string windowType, int x, int y)
{
	sf::RenderWindow newWindow(sf::VideoMode(x, y), windowType);
	tgui::Gui gui = tgui::Gui(newWindow);
	gui.setGlobalFont("../TGUI/fonts/DejaVuSans.ttf");

	tgui::Picture::Ptr backgroundImage(gui);
	backgroundImage->load("../imgs/background.jpg");
	backgroundImage->setSize(x, y);

	if (windowType == "rules")
	{
		backgroundImage->load("../imgs/rules.jpg");

		tgui::Button::Ptr acceptBtn(gui);
		acceptBtn->load("../TGUI/widgets/Black.conf");
		acceptBtn->setSize(100, 50);
		acceptBtn->setPosition(800, 300);
		acceptBtn->setText("Got it!");
		acceptBtn->bindCallback(tgui::Button::LeftMouseClicked);
		acceptBtn->setCallbackId(127);
	}
	else
	{
		tgui::Label::Ptr mainLbl(gui);
		mainLbl->load("../TGUI/widgets/Black.conf");
		mainLbl->setBackgroundColor(sf::Color(255, 255, 255, 255));
		mainLbl->setTextSize(20);
		mainLbl->setSize(140, 18);
		mainLbl->setPosition(130, 50);
		mainLbl->setText("Are you sure?");

		tgui::Button::Ptr replyYes(gui);
		replyYes->load("../TGUI/widgets/Black.conf");
		replyYes->setSize(100, 50);
		replyYes->setPosition(50, 125);
		replyYes->setText("Yes");
		replyYes->bindCallback(tgui::Button::LeftMouseClicked);
		replyYes->setCallbackId(125);

		tgui::Button::Ptr replyNo(gui);
		replyNo->load("../TGUI/widgets/Black.conf");
		replyNo->setSize(100, 50);
		replyNo->setPosition(230, 125);
		replyNo->setText("No");
		replyNo->bindCallback(tgui::Button::LeftMouseClicked);
		replyNo->setCallbackId(126);
	}

	while (newWindow.isOpen())
	{
		sf::Event event;
		while (newWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				newWindow.close();

			// Pass the event to all the widgets (if there would be widgets)
			gui.handleEvent(event);
		}

		tgui::Callback callback;
		while (gui.pollCallback(callback))
		{
			if (callback.id == 125 && windowType == "reset")
			{
				resetGame();
				newWindow.close();
			}
			if (callback.id == 125 && windowType == "quit")
			{
				newWindow.close();
				this->gameClosed = true;
			}
			else
			{
				newWindow.close();
			}
		}
		newWindow.clear();

		gui.draw();

		newWindow.display();
	}
}

/* 
This method will explore in every direction, after clicking on a piece, the 
tiles the piece can move to, and will then highlight them.
@param start : the tile ID of the clicked piece.
*/
void UI_Board::checkMovementRange(int start)
{
	int callbackId = start;

	if (!findIntInArray(Constants::TOP_BORDER_POSITIONS, 9, start))
	{
		callbackId--;

		while (gameBoard->isTileEmpty(gameBoard->tiles.at(callbackId)))
		{
			tgui::Picture::Ptr &newPic = picArray[callbackId];
			newPic->load("../imgs/HNEF_TILE_BLANK_HIGHLIGHT.gif");
			this->pieceMovementRange.push_back(callbackId);

			if (findIntInArray(Constants::TOP_BORDER_POSITIONS, 9, callbackId))
			{
				break;
			}
			callbackId--;
		}

		specialKingMovement(start, callbackId);
	}

	callbackId = start + 1;

	if (!findIntInArray(Constants::BOTTOM_BORDER_POSITIONS, 9, start))
	{
		while (gameBoard->isTileEmpty(gameBoard->tiles.at(callbackId)))
		{
			tgui::Picture::Ptr &newPic = picArray[callbackId];
			newPic->load("../imgs/HNEF_TILE_BLANK_HIGHLIGHT.gif");
			this->pieceMovementRange.push_back(callbackId);

			if (findIntInArray(Constants::BOTTOM_BORDER_POSITIONS, 9, callbackId))
			{
				break;
			}
			callbackId++;
		}

		specialKingMovement(start, callbackId);
	}

	callbackId = start - 11;
	if (callbackId >= 0)
	{
		while (gameBoard->isTileEmpty(gameBoard->tiles.at(callbackId)))
		{
			tgui::Picture::Ptr &newPic = picArray[callbackId];
			newPic->load("../imgs/HNEF_TILE_BLANK_HIGHLIGHT.gif");
			this->pieceMovementRange.push_back(callbackId);
			callbackId -= 11;
			if (callbackId < 0)
				break;
		}

		if (callbackId >= 0)
		{
			specialKingMovement(start, callbackId);
		}
	}

	callbackId = start + 11;
	if (callbackId <= 120)
	{
		while (gameBoard->isTileEmpty(gameBoard->tiles.at(callbackId)))
		{
			tgui::Picture::Ptr &newPic = picArray[callbackId];
			newPic->load("../imgs/HNEF_TILE_BLANK_HIGHLIGHT.gif");
			this->pieceMovementRange.push_back(callbackId);
			callbackId += 11;
			if (callbackId > 120)
				break;
		}

		if (callbackId <= 120)
		{
			specialKingMovement(start, callbackId);
		}
	}
}

/*
Since the king can move on the throne and castles, a special exception is made
while checking for the movement range for the king. It will highlight the
throne tile as well as the castle tile, which normally are inaccessible.
@param start: the tile ID of the clicked piece, in this case, the king;
@param destination: the last tile in any given direction.
*/
void UI_Board::specialKingMovement(int start, int destination)
{
	if (this->grid.at(start) == "whiteK" && this->grid.at(destination) == "castle")
	{
		tgui::Picture::Ptr &newPic = picArray[destination];
		newPic->load("../imgs/HNEF_TILE_castle_HIGHLIGHT.gif");
		this->pieceMovementRange.push_back(destination);
	}
	if (this->grid.at(start) == "whiteK" && this->grid.at(destination) == "throne")
	{
		tgui::Picture::Ptr &newPic = picArray[destination];
		newPic->load("../imgs/HNEF_TILE_throne_HIGHLIGHT.gif");
		this->pieceMovementRange.push_back(destination);
	}
}

/*
Once the user has moved the piece, or has chosen another action, the highlighted
tiles will return to normal and no longer be highlighted.
*/
void UI_Board::emptyPreviousMovements()
{
	while (!this->pieceMovementRange.empty())
	{
		int id = this->pieceMovementRange.back();
		std::string tileType = grid[id];
		tgui::Picture::Ptr &newPic = picArray[id];

		newPic->load("../imgs/HNEF_TILE_" + tileType + ".gif");

		this->pieceMovementRange.pop_back();
	}
	
}

/*
This method will move the piece to the desired location, as long as the destination
is part of his moving range.
@param pieceToMove : the ID of the tile which contains the moving piece;
@param destination : the tile upon which the piece is moving onto.
*/
void UI_Board::movePiece(int pieceToMove, int destination)
{
	this->gameBoard->movePiece(this->gameBoard->tiles.at(pieceToMove), this->gameBoard->tiles.at(destination));
	std::string color = this->gameBoard->tiles.at(destination)->getPiece().getColor();
	tgui::Picture::Ptr &movingPic = picArray[destination];
	tgui::Picture::Ptr &blankSpace = picArray[pieceToMove];
	
	blankSpace->load("../imgs/HNEF_TILE_BLANK.gif");
	movingPic->load("../imgs/HNEF_TILE_" + color + ".gif");

	if (pieceToMove == 60)
	{
		this->grid.at(pieceToMove) = "throne";
	}
	else
		this->grid.at(pieceToMove) = "BLANK";

	this->grid.at(destination) = color;

	emptyPreviousMovements();
	updateBoard();
}

/*
After every action, the board is refreshed according to the backend board.
Every tile is updated to reflect the backend board.
*/
void UI_Board::updateBoard()
{
	this->isKingDead = true;
	int counter = 0;

	while (counter < Constants::NO_TILES)
	{
		tgui::Picture::Ptr &updatePic = picArray[counter];
		if (this->gameBoard->doesTileHavePiece(this->gameBoard->tiles.at(counter)))
		{
			updatePic->load("../imgs/HNEF_TILE_" + this->gameBoard->tiles.at(counter)->getPiece().getColor() + ".gif");
			grid.at(counter) = this->gameBoard->tiles.at(counter)->getPiece().getColor();

			if (this->gameBoard->tiles.at(counter)->getPiece().getColor() == "whiteK")
			{
				if (this->gameBoard->tileIsACastle(this->gameBoard->tiles.at(counter)))
				{
					this->didKingWin = true;
				}
				this->isKingDead = false;
			}
		}
		else
		{
			updatePic->load("../imgs/HNEF_TILE_" + this->gameBoard->tiles.at(counter)->getType() + ".gif");
			grid.at(counter) = this->gameBoard->tiles.at(counter)->getType();
		}

		counter++;
	}
	/*for (int i = 0; i < Constants::NO_TILES; i++)
	{
		tgui::Picture::Ptr &updatePic = picArray[i];
	
		if (this->gameBoard->doesTileHavePiece(this->gameBoard->tiles.at(i)))
		{
			if (this->gameBoard->tiles.at(i)->getPiece().getColor() == "black")
			{
				updatePic->load("../imgs/HNEF_TILE_black.gif");
				grid.at(i) = "black";
			}
			if (this->gameBoard->tiles.at(i)->getPiece().getColor() == "white")
			{
				updatePic->load("../imgs/HNEF_TILE_white.gif");
				grid.at(i) = "white";
			}

			if (this->gameBoard->tiles.at(i)->getPiece().getColor() == "whiteK")
			{
				if (this->gameBoard->tileIsACastle(this->gameBoard->tiles.at(i)))
				{
					this->didKingWin = true;
				}
				this->isKingDead = false;
				updatePic->load("../imgs/HNEF_TILE_whiteK.gif");
				grid.at(i) = "whiteK";
			}
			
		}
		else if (this->gameBoard->tileIsThrone(this->gameBoard->tiles.at(i)))
		{
			updatePic->load("../imgs/HNEF_TILE_throne.gif");
			grid.at(i) = "throne";
		}
		else if (this->gameBoard->tileIsACastle(this->gameBoard->tiles.at(i)))
		{
			updatePic->load("../imgs/HNEF_TILE_castle.gif");
			grid.at(i) = "castle";
		}
		else
		{
			updatePic->load("../imgs/HNEF_TILE_BLANK.gif");
			grid.at(i) = "BLANK";
		}
		
	}*/

	this->blackDeaths->setText("Black guards alive: " + this->gameBoard->getBlackPieces());
	this->whiteDeaths->setText("White guards alive: " + this->gameBoard->getWhitePieces());
	this->whoseTurnLbl->setText("Turn: " + this->whoseTurn);
}

/*
Searches in any given array of integers a specific number.
@return bool : true if number found, false if not.
@param intArray[] : the array of numbers;
@param size : the size of the array;
@param number : the number we are looking for.
*/
bool UI_Board::findIntInArray(const int intArray[], int size, int number)
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

/*
Resets the game so that it can be played anew. The backend grid
is deleted and recreated, and then simply the UI board is refreshed to
reflect the new backend board.
*/
void UI_Board::resetGame()
{
	delete this->gameBoard;
	this->gameBoard = new B_Board();
	this->whoseTurn = "black";
	this->grid.clear();
	this->generateBoard();
	this->updateBoard();
}

/*
Special method that is used when a player has won. 
It will clear the grid, and then, each second, show a word
of the phrase "YOU ARE THE WINNER" It took me awhile to figure out
how to do this, I'm pretty happy!
*/
void UI_Board::victory(std::string winner, int numberOfTimes)
{
	switch (numberOfTimes)
	{
		case 0:
		{
			for (int i = 0; i < Constants::NO_TILES; i++)
			{
				picArray[i]->load("../imgs/HNEF_TILE_BLANK.gif");
			}
			
			break;
		}
		case 1:
		{
			Sleep(1000);
			picArray[3]->load("../imgs/HNEF_TILE_WIN_" + winner + "_Y.gif");
			picArray[14]->load("../imgs/HNEF_TILE_WIN_" + winner + "_O.gif");
			picArray[25]->load("../imgs/HNEF_TILE_WIN_" + winner + "_U.gif");
			break;
		}
		case 2:
		{
			Sleep(1000);
			picArray[47]->load("../imgs/HNEF_TILE_WIN_" + winner + "_A.gif");
			picArray[58]->load("../imgs/HNEF_TILE_WIN_" + winner + "_R.gif");
			picArray[69]->load("../imgs/HNEF_TILE_WIN_" + winner + "_E.gif");
			break;
		}
		case 3:
		{
			Sleep(1000);
			picArray[91]->load("../imgs/HNEF_TILE_WIN_" + winner + "_T.gif");
			picArray[102]->load("../imgs/HNEF_TILE_WIN_" + winner + "_H.gif");
			picArray[113]->load("../imgs/HNEF_TILE_WIN_" + winner + "_E.gif");
			break;
		}
		case 4:
		{
			Sleep(1000);
			picArray[27]->load("../imgs/HNEF_TILE_WIN_" + winner + "_W.gif");
			picArray[38]->load("../imgs/HNEF_TILE_WIN_" + winner + "_I.gif");
			picArray[49]->load("../imgs/HNEF_TILE_WIN_" + winner + "_N.gif");
			picArray[60]->load("../imgs/HNEF_TILE_WIN_" + winner + "_N.gif");
			picArray[71]->load("../imgs/HNEF_TILE_WIN_" + winner + "_E.gif");
			picArray[82]->load("../imgs/HNEF_TILE_WIN_" + winner + "_R.gif");
			picArray[93]->load("../imgs/HNEF_TILE_WIN_!.gif");
			break;
		}
		
	}
	
	

	

	

	

	
	
}
