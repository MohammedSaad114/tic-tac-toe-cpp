#pragma once
#include "raylib.h"
#include <array>
#include <optional>
#include <vector>
#include <set>
#include <utility>

enum Cell{ O = -1, EMPTY = 0, X = 1 };

struct Board {
	std::vector<std::vector<Cell>> grid;

	bool isEmpty() const
	{
		for (const auto& row : grid)
			for (int cell : row)
				if (cell != EMPTY)
					return false;
		return true;
	}
};

using Move = std::pair<int, int>;
using Eval = std::pair<int, Move>; // {score, best move}
static const Move kNoMove{ -1,-1 };

class Game {
public:
	Board m_board{ std::vector<std::vector<Cell>>(3, std::vector<Cell>(3, EMPTY)) };
private:
	Cell m_current { X };
	bool m_gameOver { false };

public:
	Game(Cell currentPlayer);

	void resetBoard();
    /**
    * returns the player with the next turn
    */
	Cell player(const Board& board);

	/**
	* returns a set of all possible actions
	*/
	std::set<Move> actions(const Board& board);

	/**
	* returns a board that results from an action
	*/
	Board result(const Board& board, Move action);

	/**
	* returns a winner if there is any
	*/
	Cell winner(const Board& board);

	/**
	* checks if game is over
	*/
	bool terminal(const Board& board);

	/**
	* returns optimal action for current player
	*/
	Eval optimalAction(const Board& board);
private:
	/**
	* Max value for player (X), move with heighest score
	*/
	Eval maxValue(const Board& board);
	/**
	* returns min value for player(O), move with lowest score
	*/
	Eval minValue(const Board& board);

};



