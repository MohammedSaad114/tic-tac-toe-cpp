#include "game.h"

Game::Game(Cell currentPlayer) : m_current{currentPlayer} {}

void Game::resetBoard()
{
	m_board = { std::vector<std::vector<Cell>>(3, std::vector<Cell>(3, EMPTY)) };
}

Cell Game::player(const Board& board)
{
	if (board.isEmpty())
	{
		return X;
	}
		int countX = 0;
		int countO = 0;
		for (size_t i = 0; i < board.grid.size(); ++i)
		{
			for (size_t j = 0; j < board.grid[i].size(); ++j)
				if (board.grid[i][j] == X)
					countX++;
				else if (board.grid[i][j] == O)
					countO++;
		}
		return (countX > countO) ? O : X;
}

std::set<Move> Game::actions(const Board& board)
{
	std::set<Move> possibleActions;
	for (size_t i = 0; i < board.grid.size(); ++i)
	{
		for (size_t j = 0; j < board.grid[i].size(); ++j)
			if (board.grid[i][j] == EMPTY)
				possibleActions.insert({ i,j });
	}
	return possibleActions;
}

Board Game::result(const Board& board, Move action)
{
	int i = action.first;
	int j = action.second;
	Board newBoard = board;
	newBoard.grid[i][j] = player(board);		
	return newBoard;
}

Cell Game::winner(const Board& board)
{
	// check diagonals
	if (board.grid[0][0] != EMPTY && board.grid[0][0] == board.grid[1][1] && board.grid[1][1] == board.grid[2][2]) return board.grid[1][1];
	if (board.grid[2][0] != EMPTY && board.grid[2][0] == board.grid[1][1] && board.grid[1][1] == board.grid[0][2]) return board.grid[1][1];
			
	// check rows
	for (size_t i = 0; i < 3; ++i)
		if (board.grid[i][0] != EMPTY && board.grid[i][0] == board.grid[i][1] && board.grid[i][1] == board.grid[i][2]) return board.grid[i][0];

	// check columns
	for (size_t j = 0; j < 3; ++j)
		if (board.grid[0][j] != EMPTY && board.grid[0][j] == board.grid[1][j] && board.grid[1][j] == board.grid[2][j]) return board.grid[0][j];

	return EMPTY;
}

bool Game::terminal(const Board& board)
{
	if (winner(board) != EMPTY)
		return true;
	if (actions(board).empty())
		return true;		
	return false;
	
}



Eval Game::optimalAction(const Board& board)
{
	return (player(board) == X) ? maxValue(board) : minValue(board);
}

Eval Game::maxValue(const Board& board)
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

Eval Game::minValue(const Board& board)
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
