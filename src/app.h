#pragma once
#include "raylib.h"
#include "game.h"

enum class State { Menu, Playing };

struct App {
	// window
	int width{}, height{};

	// game
	State state{ State::Menu };
	int user{ 0 };
	Board board = initialState();
	bool  aiTurn{ false };
	double aiTimer{ 0.0 };
	double aiDelay{ 0.5 };

	// layout
	int tileSize{ 80 };
	Vector2 origin{};
	Rectangle tiles[3][3]{};
};

/**
* init ui and game info
*/
void initApp(App& app, int w, int h);

/**
* handles input, update of one frame
*/
void frame(App& app);