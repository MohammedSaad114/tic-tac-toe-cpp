#pragma once
#include "game.h"

enum class State { Menu, Playing };

class App {
private:
	// window
	int m_width{}, m_height{};
	// game
	Game m_game;
	State m_state = State::Menu;
	int m_user = 0;
	bool  m_aiTurn = false;
	double m_aiTimer = 0.0;
	double m_aiDelay = 0.5;

	// layout
	int m_tileSize = 80;
	Vector2 m_origin{};
	Rectangle m_tiles[3][3]{};
public:
	App(int w, int h);
	/**
	* handles input, update of one frame
	*/
	void frame();
	void drawMenu();
	void drawCenteredText(const char* s, int y, int size, Color c, int winW);
	void drawBoard();
	void playAgain();
	void updatePlaying();
	void runGame();
};

