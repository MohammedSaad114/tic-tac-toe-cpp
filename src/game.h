#pragma once
#include "raylib.h"
#include <array>
#include <optional>
#include <vector>
#include <set>
#include <utility>

enum Cell{ EMPTY = 0, X = 1, O = -1};

using Board = std::vector<std::vector<int>>;
using Move = std::pair<int, int>;
using Eval = std::pair<int, Move>; // {score, best move}

struct Game {
    Board board{};
    int current = X;
    bool gameOver = false;
};

/**
* returns initial empty 3x3 board
*/
Board initialState();

/**
* returns the player with the next turn
*/
int player(const Board& board);

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
int winner(const Board& board);

/**
* checks if game is over
*/
bool terminal(const Board& board);

/**
* returns optimal action for current player
*/
Eval optimalAction(const Board& board);