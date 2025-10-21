#include "game.h"
#include <stdexcept>

Board initialState()
{
	return std::vector<std::vector<int>>(3, std::vector<int>(3, EMPTY));
}

int player(const Board& board)
{
	if (board == initialState())
	{
		return X;
	}
	else
	{
		int countX = 0;
		int countO = 0;
		for (size_t i = 0; i < board.size(); ++i)
		{
			for (size_t j = 0; j < board[i].size(); ++j)
			{
				if (board[i][j] == X)
					countX++;
				else if (board[i][j] == O)
					countO++;
			}
		}
		return (countX > countO) ? O : X;
	}
}

std::set<Move> actions(const Board& board)
{
	std::set<Move> possibleActions;
	for (size_t i = 0; i < board.size(); ++i)
	{
		for (size_t j = 0; j < board[i].size(); ++j)
		{
			if (board[i][j] == EMPTY)
				possibleActions.insert({ i,j });
		}
	}
	return possibleActions;
}

Board result(const Board& board, Move action)
{
	int i = action.first;
	int j = action.second;
	if (actions(board).count({i, j}) == 0)
	{
		throw std::runtime_error("Invalid action: Cell is not empty.");
	} 
	else
	{
		Board newBoard = board;
		newBoard[i][j] = player(board);
		return newBoard;
	}
}

int winner(const Board& board)
{
	// check diagonals
	if (board[0][0] != EMPTY && board[0][0] == board[1][1] && board[1][1] == board[2][2]) return board[1][1];
	if (board[2][0] != EMPTY && board[2][0] == board[1][1] && board[1][1] == board[0][2]) return board[1][1];
			
	// check rows
	for (size_t i = 0; i < 3; ++i)
		if (board[i][0] != EMPTY && board[i][0] == board[i][1] && board[i][1] == board[i][2]) return board[i][0];

	// check columns
	for (size_t j = 0; j < 3; ++j)
		if (board[0][j] != EMPTY && board[0][j] == board[1][j] && board[1][j] == board[2][j]) return board[0][j];

	return EMPTY;
}

bool terminal(const Board& board)
{
	if (winner(board) != EMPTY)
	{
		return true;
	}
	else
	{
		if (actions(board).empty())
			return true;
		return false;
	}
}

// forward declarations
static Eval maxValue(const Board& board);
static Eval minValue(const Board& board);
static const Move kNoMove{ -1,-1 };

Eval optimalAction(const Board& board)
{
	return (player(board) == X) ? maxValue(board) : minValue(board);
}

// Max value for player (X), move with heighest score
static Eval maxValue(const Board& board)
{
	// if game ends, return winner val and no action
	if (terminal(board))
		return { winner(board), kNoMove };

	int best = std::numeric_limits<int>::min();
	Move bestMove = kNoMove;

	for (const Move& a : actions(board))
	{
		// opponent's best moves
		Eval child = minValue(result(board, a));
		int score = child.first;

		if (score > best)
		{
			best = score;
			bestMove = a;
			// perfect win´for X found
			if (best == 1) break; 
		}
	}
	return { best, bestMove };

}

// returns min value for player (O), move with lowest score
static Eval minValue(const Board& board)
{
	// if game ends, return winner val and no action
	if (terminal(board))
		return { winner(board), kNoMove };

	int best = std::numeric_limits<int>::max();
	Move bestMove = kNoMove;

	for (const Move& a : actions(board))
	{
		// opponent's best moves
		Eval child = maxValue(result(board, a));
		int score = child.first;

		if (score < best)
		{
			best = score;
			bestMove = a;
			// perfect win for O
			if (best == -1) break;
		}
	}
	return { best, bestMove };
}
