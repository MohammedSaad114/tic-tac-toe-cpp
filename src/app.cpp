#include "app.h"

App::App(int w, int h): m_width{w}, m_height{h}, m_game {Game(Cell::EMPTY)} {
    m_origin = { m_width / 2.0f - 1.5f * m_tileSize, m_height / 2.0f - 1.5f * m_tileSize };
};

void App::drawCenteredText(const char* s, int y, int size, Color c, int winW)
{
   
    Vector2 size = MeasureTextEx(a.uiFont, s, fontSize, 1.0f);
    Vector2 pos{
       r.x + (r.width - size.x) / 2.0f,
       r.y + (r.height - size.y) / 2.0f
    };
    DrawTextEx(a.uiFont, s, pos, fontSize, 1.0f, c);
}

void App::drawMenu()
{
    // Title
    drawCenteredText("Play Tic-Tac-Toe", 50, 40, RAYWHITE, m_width);

    // Buttons
    Rectangle bx{ m_width / 8.0f,   m_height / 2.0f, m_width / 4.0f, 50.0f };
    Rectangle bo{ 5 * m_width / 8.0f, m_height / 2.0f, m_width / 4.0f, 50.0f };

    DrawRectangleRec(bx, RAYWHITE);
    int twX = MeasureText("Play as X", 24);
    DrawText("Play as X", bx.x + (bx.width - twX) / 2, bx.y + (bx.height - 24) / 2, 24, BLACK);

    DrawRectangleRec(bo, RAYWHITE);
    int twO = MeasureText("Play as O", 24);
    DrawText("Play as O", bo.x + (bo.width - twO) / 2, bo.y + (bo.height - 24) / 2, 24, BLACK);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 m = GetMousePosition();
        if (CheckCollisionPointRec(m, bx)) { m_user = X; m_game.m_board; m_state = State::Playing; }
        else if (CheckCollisionPointRec(m, bo)) { m_user = O; m_game.m_board; m_state = State::Playing; }
    }
}

void App::drawBoard()
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
		{
			Rectangle r{ m_origin.x + j * m_tileSize, m_origin.y + i * m_tileSize,
						 (float)m_tileSize, (float)m_tileSize };
			m_tiles[i][j] = r;
			DrawRectangleLinesEx(r, 3.0f, RAYWHITE);

			int v = m_game.m_board.grid[i][j];
			if (v != EMPTY)
			{
				const char* s = (v == X) ? "X" : "O";
				int fs = 60;
				int tw = MeasureText(s, fs);
				DrawText(s, (int)(r.x + (r.width - tw) / 2),
							(int)(r.y + (r.height - fs) / 2), fs, RAYWHITE);
			}
		}
}

void App::playAgain()
{
	Rectangle again{ m_width / 3.0f, m_height - 65.0f, m_width / 3.0f, 50.0f };
	DrawRectangleRec(again, RAYWHITE);
	int tw = MeasureText("Play Again", 24);
	DrawText("Play Again", again.x + (again.width - tw) / 2, again.y + (again.height - 24) / 2, 24, BLACK);
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		Vector2 m = GetMousePosition();
		if (CheckCollisionPointRec(m, again))
		{
			m_state = State::Menu;
			m_user = 0;
			m_game.resetBoard();
			m_aiTurn = false;
		}
	}
}

void App::updatePlaying()
{
	bool over = m_game.terminal(m_game.m_board);
	int turn = m_game.player(m_game.m_board);

	// Status
	const char* status = nullptr;
	if (over)
	{
		int w = m_game.winner(m_game.m_board);
		status = (w == 0) ? "Game Over: Tie." : (w == X ? "Game Over: X wins." : "Game Over: O wins.");
	}
	else
	{
		status = (m_user == turn) ? ((m_user == X) ? "Play as X" : "Play as O")
			: "Computer thinking...";
	}
	drawCenteredText(status, 20, 32, RAYWHITE, m_width);

	// Board
	drawBoard();

	// AI move
	if (!over && m_user != turn)
	{
		if (!m_aiTurn) { m_aiTurn = true; m_aiTimer = GetTime(); }
		else if (GetTime() - m_aiTimer >= m_aiDelay)
		{
			auto best = m_game.optimalAction(m_game.m_board);        // {score, Move}
			if (best.second != kNoMove) m_game.m_board = m_game.result(m_game.m_board, best.second);
			m_aiTurn = false;
		}
	}

	// Human move
	if (!over && m_user == turn && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		Vector2 m = GetMousePosition();
		for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j)
		{
			if (m_game.m_board.grid[i][j] == EMPTY && CheckCollisionPointRec(m, m_tiles[i][j]))
			{
				m_game.m_board = m_game.result(m_game.m_board, { i,j });
			}
		}
	}

	// Play again UI
	if (over) playAgain();
}

void App::frame()
{
	switch (App::m_state)
	{
		case State::Menu: drawMenu(); break;
		case State::Playing: updatePlaying(); break;
	}
}

void App::runGame()
{   
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);	
	// Create the window and OpenGL context
	InitWindow(m_width, m_height, "Tic Tac Toe");
	SetTargetFPS(60);

	// game loop
	while (!WindowShouldClose()) {		
		// run the loop untill the user presses ESCAPE or presses the Close button on the window
		// drawing
		BeginDrawing();
		ClearBackground(BLACK);
		frame();
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}
	// destroy the window and cleanup the OpenGL context
	CloseWindow();
}

