#include "stdafx.h"
#include "CppUnitTest.h"
#include <cassert>
#include "../Hnefatafl - backend/B_Board.h"
#include "../Hnefatafl - backend/B_Tile.h"
#include "../Hnefatafl - backend/B_Piece.h"
#include "../Hnefatafl - backend/Constants.h"
/*
This page contains every unit test that I have done.
Normally, it covers every different case of movement, attacking, and basically
all the backend code.
@author: Kevin Tanguay
*/
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Constants;

namespace UnitTest
{
	TEST_CLASS(unittest1)
	{
	public:

		TEST_METHOD(Test_Board_Creation)
		{
			B_Board* testBoard = new B_Board();
			Assert::IsNotNull(testBoard);
			delete testBoard;
		}

		TEST_METHOD(Test_Constant)
		{
			int boardSize = Constants::BOARD_SIZE;
			Assert::AreEqual(boardSize, 11);
		}

		TEST_METHOD(Test_Board_Initialisation)
		{
			B_Board testBoard;
			Assert::AreEqual(testBoard.getSize(), 11);
		}

		TEST_METHOD(Test_Board_Initialisation_With_11x11_Tiles)
		{
			B_Board testBoard;
			int size = testBoard.tiles.size();
			Assert::AreEqual(size, 121);
		}

		TEST_METHOD(Test_Board_Contains_Good_Amount_Of_Castle_Tiles)
		{
			B_Board testBoard;
			int castleCount = 0;
			for (int i = 0; i < Constants::NO_TILES; i++)
			{
				if (testBoard.tiles.at(i)->getType() == "castle")
				{
					castleCount++;
				}
			}

			Assert::AreEqual(Constants::NO_CASTLES, castleCount);
		}

		TEST_METHOD(Test_Board_Contains_Good_Amount_Of_Black_Pieces)
		{
			B_Board testBoard;
			int blackCount = 0;
			B_Piece testPiece("white");

			for (int i = 0; i < Constants::NO_TILES; i++)
			{
				testPiece = testBoard.tiles.at(i)->getPiece();
				if (testPiece.ifExists() && testPiece.getColor() == "black")
				{
					blackCount++;
				}
			}

			Assert::AreEqual(Constants::NO_BLACK_PIECES, blackCount);
		}

		TEST_METHOD(Test_Board_Contains_Good_Amount_Of_White_Pieces)
		{
			B_Board testBoard;
			int whiteCount = 0;
			B_Piece testPiece("black");

			for (int i = 0; i < Constants::NO_TILES; i++)
			{
				testPiece = testBoard.tiles.at(i)->getPiece();
				if (testPiece.ifExists() && testPiece.getColor() == "white")
				{
					whiteCount++;
				}
			}

			Assert::AreEqual(Constants::NO_WHITE_PIECES, whiteCount);
		}

		TEST_METHOD(Test_Board_Contains_Good_Amount_Of_Empty_Spaces)
		{
			B_Board testBoard;
			int emptyCount = 0;
			int totalEmptySpaces = Constants::NO_TILES - (Constants::NO_BLACK_PIECES + Constants::NO_WHITE_PIECES) - 1;
			for (int i = 0; i < Constants::NO_TILES; i++)
			{
				if (!testBoard.tiles.at(i)->getPiece().ifExists())
				{
					emptyCount++;
				}
			}

			Assert::AreEqual(totalEmptySpaces, emptyCount);
		}

		TEST_METHOD(Test_Board_Contains_Throne_In_Middle)
		{
			B_Board testBoard;
			int blackCount = 0;
			Assert::IsTrue(testBoard.tiles.at(60)->getType() == "throne");
		}

		TEST_METHOD(Test_Empty_Tile_Creation)
		{
			B_Tile newTile("", 0);
			std::string empty = "";
			Assert::AreEqual(empty, newTile.getType());
		}

		TEST_METHOD(Test_Tile_Visual_Positioning_1)
		{
			B_Tile newTile("", 1);
			std::string xyPlacementExpected = "01";
			std::string xyPlacement = std::to_string(newTile.getX()) + std::to_string(newTile.getY());
			Assert::IsTrue(xyPlacement == xyPlacementExpected);
		}

		TEST_METHOD(Test_Tile_Visual_Positioning_2)
		{
			B_Tile newTile("", 18);
			std::string xyPlacementExpected = "17";
			std::string xyPlacement = std::to_string(newTile.getX()) + std::to_string(newTile.getY());
			Assert::IsTrue(xyPlacement == xyPlacementExpected);
		}

		TEST_METHOD(Test_Tile_Visual_Positioning_3)
		{
			B_Tile newTile("", 73);
			std::string xyPlacementExpected = "67";
			std::string xyPlacement = std::to_string(newTile.getX()) + std::to_string(newTile.getY());
			Assert::IsTrue(xyPlacement == xyPlacementExpected);
		}

		TEST_METHOD(Test_Tile_Visual_Positioning_4)
		{
			B_Tile newTile("", 120);
			std::string xyPlacementExpected = "1010";
			std::string xyPlacement = std::to_string(newTile.getX()) + std::to_string(newTile.getY());
			Assert::IsTrue(xyPlacement == xyPlacementExpected);
		}

		TEST_METHOD(Test_Castle_Tile_Creation)
		{
			B_Tile newTile("castle", 0);
			std::string castle = "castle";
			Assert::AreEqual(castle, newTile.getType());
		}

		TEST_METHOD(Test_Throne_Tile_Creation)
		{
			B_Tile newTile("throne", 0);
			std::string throne = "throne";
			Assert::AreEqual(throne, newTile.getType());
		}

		TEST_METHOD(Test_Tile_SetType)
		{
			B_Tile newTile("throne", 0);
			newTile.setType("castle");
			Assert::IsTrue(newTile.getType() == "castle");
		}

		TEST_METHOD(Test_Tile_SetPiece)
		{
			B_Tile newTile("throne", 0);
			B_Piece newPiece("black");
			newTile.setPiece(newPiece);
			Assert::IsTrue(newTile.getPiece().getColor() == newPiece.getColor());
		}

		TEST_METHOD(Test_White_Piece_Creation)
		{
			B_Piece* newPiece = new B_Piece("white");

			Assert::IsTrue(newPiece->getColor() == "white");
		}

		TEST_METHOD(Test_Black_Piece_Creation)
		{
			B_Piece* newPiece = new B_Piece("black");

			Assert::IsTrue(newPiece->getColor() == "black");
		}

		TEST_METHOD(Test_Move_Piece_Deletes_Old_Piece_Position)
		{
			B_Board testBoard;
			B_Piece pieceToMove = testBoard.tiles.at(5)->getPiece();
			testBoard.movePiece(testBoard.tiles.at(5), testBoard.tiles.at(27));
			B_Piece shouldBeEmpty = testBoard.tiles.at(5)->getPiece();

			Assert::IsFalse(shouldBeEmpty.ifExists());
		}

		TEST_METHOD(Test_Move_Piece_Should_Be_At_New_Position)
		{
			B_Board testBoard;
			B_Piece pieceToMove = testBoard.tiles.at(5)->getPiece();
			testBoard.movePiece(testBoard.tiles.at(5), testBoard.tiles.at(27));
			B_Piece movedPiece = testBoard.tiles.at(27)->getPiece();

			Assert::IsTrue(movedPiece.getColor() == pieceToMove.getColor());
		}

		TEST_METHOD(Test_doesTileHavePiece_Should_Return_True_When_Tile_Occupied)
		{
			B_Board testBoard;
			Assert::IsTrue(testBoard.doesTileHavePiece(testBoard.tiles.at(5)));
		}

		TEST_METHOD(Test_doesTileHavePiece_Should_Return_False_When_Tile_Innoccupied)
		{
			B_Board testBoard;
			Assert::IsFalse(testBoard.doesTileHavePiece(testBoard.tiles.at(2)));
		}

		TEST_METHOD(Test_CheckMoveDirection_Should_Give_Up)
		{
			B_Board testBoard;
			std::string moveDirection = testBoard.checkMoveDirection(3, 2);
			std::string up = "up";
			Assert::AreEqual(moveDirection, up);
		}

		TEST_METHOD(Test_CheckMoveDirection_Should_Give_Down)
		{
			B_Board testBoard;
			std::string moveDirection = testBoard.checkMoveDirection(7, 8);
			std::string down = "down";
			Assert::AreEqual(moveDirection, down);
		}

		TEST_METHOD(Test_CheckMoveDirection_Should_Give_Left)
		{
			B_Board testBoard;
			std::string moveDirection = testBoard.checkMoveDirection(33, 22);
			std::string left = "left";
			Assert::AreEqual(moveDirection, left);
		}

		TEST_METHOD(Test_CheckMoveDirection_Should_Give_Right)
		{
			B_Board testBoard;
			std::string moveDirection = testBoard.checkMoveDirection(3, 14);
			std::string right = "right";
			Assert::AreEqual(moveDirection, right);
		}

		TEST_METHOD(Test_IsThereAnEnemy_Should_Return_True)
		{
			B_Board testBoard;
			Assert::IsTrue(testBoard.isThereAnEnemy(4, "white", false));
		}

		TEST_METHOD(Test_IsThereAnEnemy_Should_Return_False)
		{
			B_Board testBoard;
			Assert::IsFalse(testBoard.isThereAnEnemy(4, "black", false));
		}

		TEST_METHOD(Test_IsThereAnEnemy_On_Empty_Tile_Should_Return_False)
		{
			B_Board testBoard;
			Assert::IsFalse(testBoard.isThereAnEnemy(1, "white", false));
		}

		TEST_METHOD(Test_DestroyEnemy_Should_Remove_Selected_Piece)
		{
			B_Board testBoard;
			
			testBoard.destroyEnemy(testBoard.tiles.at(4));

			B_Piece deletedPiece = testBoard.tiles.at(4)->getPiece();
			Assert::IsTrue(!deletedPiece.ifExists());
		}

		TEST_METHOD(Test_Destroy_Black_Enemy_Should_Decrement_Black_Pieces)
		{
			B_Board testBoard;

			testBoard.destroyEnemy(testBoard.tiles.at(4));
			
			Assert::AreNotEqual(atoi(testBoard.getBlackPieces().c_str()), Constants::NO_BLACK_PIECES);
		}

		TEST_METHOD(Test_Destroy_White_Enemy_Should_Decrement_White_Pieces)
		{
			B_Board testBoard;

			testBoard.destroyEnemy(testBoard.tiles.at(50));

			Assert::AreNotEqual(atoi(testBoard.getWhitePieces().c_str()), Constants::NO_WHITE_PIECES);
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Horizontal_Right_Side_Left)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(3), testBoard.tiles.at(47));
			testBoard.movePiece(testBoard.tiles.at(113), testBoard.tiles.at(69));

			B_Piece deletedWhitePiece = testBoard.tiles.at(58)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Horizontal_Right_Side_Down)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(3), testBoard.tiles.at(47));
			testBoard.movePiece(testBoard.tiles.at(66), testBoard.tiles.at(69));

			B_Piece deletedWhitePiece = testBoard.tiles.at(58)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Horizontal_Right_Side_Up)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(7), testBoard.tiles.at(51));
			testBoard.movePiece(testBoard.tiles.at(76), testBoard.tiles.at(73));

			B_Piece deletedWhitePiece = testBoard.tiles.at(62)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Vertical_Down_Side_Up)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(33), testBoard.tiles.at(37));
			testBoard.movePiece(testBoard.tiles.at(43), testBoard.tiles.at(39));

			B_Piece deletedWhitePiece = testBoard.tiles.at(38)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Vertical_Down_Side_Right)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(33), testBoard.tiles.at(37));
			testBoard.movePiece(testBoard.tiles.at(6), testBoard.tiles.at(39));

			B_Piece deletedWhitePiece = testBoard.tiles.at(38)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Vertical_Down_Side_Left)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(77), testBoard.tiles.at(81));
			testBoard.movePiece(testBoard.tiles.at(116), testBoard.tiles.at(83));

			B_Piece deletedWhitePiece = testBoard.tiles.at(82)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Horizontal_Left_Side_Right)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(113), testBoard.tiles.at(69));
			testBoard.movePiece(testBoard.tiles.at(3), testBoard.tiles.at(47));

			B_Piece deletedWhitePiece = testBoard.tiles.at(58)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Horizontal_Left_Side_Down)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(66), testBoard.tiles.at(69));
			testBoard.movePiece(testBoard.tiles.at(3), testBoard.tiles.at(47));

			B_Piece deletedWhitePiece = testBoard.tiles.at(58)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Horizontal_Left_Side_Up)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(76), testBoard.tiles.at(73));
			testBoard.movePiece(testBoard.tiles.at(7), testBoard.tiles.at(51));

			B_Piece deletedWhitePiece = testBoard.tiles.at(62)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Vertical_Up_Side_Down)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(43), testBoard.tiles.at(39));
			testBoard.movePiece(testBoard.tiles.at(33), testBoard.tiles.at(37));

			B_Piece deletedWhitePiece = testBoard.tiles.at(38)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Vertical_Up_Side_Right)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(6), testBoard.tiles.at(39));
			testBoard.movePiece(testBoard.tiles.at(33), testBoard.tiles.at(37));

			B_Piece deletedWhitePiece = testBoard.tiles.at(38)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Vertical_Up_Side_Left)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(116), testBoard.tiles.at(83));
			testBoard.movePiece(testBoard.tiles.at(77), testBoard.tiles.at(81));

			B_Piece deletedWhitePiece = testBoard.tiles.at(82)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_Attack_With_King_Should_Destroy_Enemy_With_White_Assist)
		{
			B_Board testBoard;
			B_Piece newPiece("whiteK");
			testBoard.tiles.at(28)->setPiece(newPiece);

			testBoard.movePiece(testBoard.tiles.at(48), testBoard.tiles.at(15));
			testBoard.movePiece(testBoard.tiles.at(28), testBoard.tiles.at(17));

			B_Piece deletedWhitePiece = testBoard.tiles.at(16)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_Attack_With_White_Should_Destroy_Enemy_With_King_Assist)
		{
			B_Board testBoard;
			B_Piece newPiece("whiteK");
			testBoard.tiles.at(17)->setPiece(newPiece);

			testBoard.movePiece(testBoard.tiles.at(48), testBoard.tiles.at(15));

			B_Piece deletedWhitePiece = testBoard.tiles.at(16)->getPiece();
			Assert::IsTrue(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_Attack_With_Black_Should_Not_Destroy_Enemy_With_King_Assist)
		{
			B_Board testBoard;
			B_Piece newPiece("whiteK");
			testBoard.tiles.at(39)->setPiece(newPiece);

			testBoard.movePiece(testBoard.tiles.at(4), testBoard.tiles.at(37));

			B_Piece deletedWhitePiece = testBoard.tiles.at(38)->getPiece();
			Assert::IsFalse(!deletedWhitePiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_White_Destroy_Black_On_Left_Board_Edge)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(48), testBoard.tiles.at(15));
			testBoard.movePiece(testBoard.tiles.at(50), testBoard.tiles.at(17));

			B_Piece deletedBlackPiece = testBoard.tiles.at(16)->getPiece();
			Assert::IsTrue(!deletedBlackPiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_White_Destroy_Black_On_Right_Board_Edge)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(70), testBoard.tiles.at(103));
			testBoard.movePiece(testBoard.tiles.at(72), testBoard.tiles.at(105));

			B_Piece deletedBlackPiece = testBoard.tiles.at(104)->getPiece();
			Assert::IsTrue(!deletedBlackPiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Enemy_Should_Not_Die_On_Top_Border)
		{
			B_Board testBoard;
			B_Piece newPiece("white");
			testBoard.tiles.at(76)->setPiece(newPiece);
			testBoard.movePiece(testBoard.tiles.at(82), testBoard.tiles.at(78));

			B_Piece deletedBlackPiece = testBoard.tiles.at(77)->getPiece();
			Assert::IsFalse(!deletedBlackPiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Enemy_Should_Not_Die_On_Bottom_Border)
		{
			B_Board testBoard;
			B_Piece newPiece("white");
			testBoard.tiles.at(77)->setPiece(newPiece);
			testBoard.movePiece(testBoard.tiles.at(72), testBoard.tiles.at(75));

			B_Piece deletedBlackPiece = testBoard.tiles.at(76)->getPiece();
			Assert::IsFalse(!deletedBlackPiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Castle_Will_Destroy_Horizontal_Enemy)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(16), testBoard.tiles.at(11));
			testBoard.movePiece(testBoard.tiles.at(38), testBoard.tiles.at(27));
			testBoard.movePiece(testBoard.tiles.at(27), testBoard.tiles.at(22));

			B_Piece deletedBlackPiece = testBoard.tiles.at(11)->getPiece();
			Assert::IsTrue(!deletedBlackPiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Castle_Will_Destroy_Vertical_Enemy)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(3), testBoard.tiles.at(1));
			testBoard.movePiece(testBoard.tiles.at(58), testBoard.tiles.at(57));
			testBoard.movePiece(testBoard.tiles.at(57), testBoard.tiles.at(2));

			B_Piece deletedBlackPiece = testBoard.tiles.at(1)->getPiece();
			Assert::IsTrue(!deletedBlackPiece.ifExists());
		}

		TEST_METHOD(Test_CheckAdjacentEnemy_Throne_Will_Destroy_Vertical_Enemy)
		{
			B_Board testBoard;

			testBoard.movePiece(testBoard.tiles.at(50), testBoard.tiles.at(39));
			testBoard.movePiece(testBoard.tiles.at(61), testBoard.tiles.at(50));
			testBoard.movePiece(testBoard.tiles.at(62), testBoard.tiles.at(51));
			testBoard.movePiece(testBoard.tiles.at(64), testBoard.tiles.at(61));
			testBoard.movePiece(testBoard.tiles.at(51), testBoard.tiles.at(62));

			B_Piece deletedBlackPiece = testBoard.tiles.at(61)->getPiece();
			Assert::IsTrue(!deletedBlackPiece.ifExists());
		}

		TEST_METHOD(Test_AttackKing_Will_Destroy_King)
		{
			B_Board testBoard;
			B_Piece newPiece("whiteK");
			testBoard.tiles.at(24)->setPiece(newPiece);

			testBoard.movePiece(testBoard.tiles.at(3), testBoard.tiles.at(25));
			testBoard.movePiece(testBoard.tiles.at(16), testBoard.tiles.at(13));
			testBoard.movePiece(testBoard.tiles.at(33), testBoard.tiles.at(35));
			testBoard.movePiece(testBoard.tiles.at(56), testBoard.tiles.at(23));

			B_Piece deletedKingPiece = testBoard.tiles.at(24)->getPiece();
			Assert::IsTrue(!deletedKingPiece.ifExists());
		}

		
	};
}