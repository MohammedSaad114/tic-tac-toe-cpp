/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "game.h"

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	
	// Create the window and OpenGL context
	const int WIDTH = 600;
	const int HEIGHT = 400;
	InitWindow(WIDTH, HEIGHT, "Tic Tac Toe");
	SetTargetFPS(60);

	// Game info
	int user = 0;
	Board board = initialState();
	bool aiTurn = false;
	double aiTimer = 0.0;
	const double AI_DELAY = 0.5;

	// Grid layout (centered)
	const int tileSize = 80;
	const Vector2 origin{ WIDTH / 2.0f - 1.5f * tileSize, HEIGHT / 2.0f - 1.5f * tileSize };
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();
		ClearBackground(BLACK);

		// Game starts
		if (user == 0) 
		{
			// Title
			const char* title = "Play Tic-Tac-Toe";
			int tfs = 40;
			int tw = MeasureText(title, tfs);
			DrawText(title, (600 - tw) / 2, 50, tfs, RAYWHITE);

			// Buttons rectangles
			Rectangle buttonX = Rectangle{ WIDTH/8.0f, HEIGHT/2.0f, WIDTH/4.0f, 50.0f };
			Rectangle buttonO = Rectangle{ 5*WIDTH/8.0f, HEIGHT/2.0f, WIDTH/4.0f, 50.0f };
			
			// Draw buttons
			int fs = 24;

			DrawRectangleRec(buttonX, RAYWHITE);
			int twX = MeasureText("Play as X", fs);
			DrawText("Play as X", buttonX.x + (buttonX.width - twX) / 2, buttonX.y + (buttonX.height - fs) / 2, fs, BLACK);

			DrawRectangleRec(buttonO, RAYWHITE);
			int twO = MeasureText("Play as O", fs);
			DrawText("Play as O", buttonO.x + (buttonO.width - twO) / 2, buttonO.y + (buttonO.height - fs) / 2, fs, BLACK);

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				Vector2 mp = GetMousePosition();
				if (CheckCollisionPointRec(mp, buttonX))
				{
					user = X;
				}
				else if (CheckCollisionPointRec(mp, buttonO))
				{
					user = O;
				}
			}
		}

        else
        {
            // compute state up front
            bool game_over = terminal(board);
            int  turn = player(board);

            // pick status text (never nullptr)
            const char* status = nullptr;
            if (game_over)
            {
                int w = winner(board);
                status = (w == 0) ? "Game Over: Tie."
                    : (w == X ? "Game Over: X wins." : "Game Over: O wins.");
            }
            else
            {
                status = (user == turn) ? ((user == X) ? "Play as X" : "Play as O")
                    : "Computer thinking...";
            }

            // draw status
            const int tfs = 32;
            int tw = MeasureText(status, tfs);
            DrawText(status, (WIDTH - tw) / 2, 20, tfs, RAYWHITE);

            // draw board
            const int tileSize = 80;
            const Vector2 origin{ WIDTH / 2.0f - 1.5f * tileSize, HEIGHT / 2.0f - 1.5f * tileSize };

            Rectangle tiles[3][3];
            for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j)
            {
                Rectangle r{ origin.x + j * tileSize, origin.y + i * tileSize,
                             (float)tileSize, (float)tileSize };
                tiles[i][j] = r;
                DrawRectangleLinesEx(r, 3.0f, RAYWHITE);

                int cell = board[i][j];
                if (cell != EMPTY)
                {
                    const char* s = (cell == X) ? "X" : "O";
                    const int fs = 60;
                    int w = MeasureText(s, fs);
                    DrawText(s, (int)(r.x + (r.width - w) / 2),
                                (int)(r.y + (r.height - fs) / 2), fs, RAYWHITE);
                }
            }

            // AI move (use move sentinel, not score)
            const Move NO_MOVE{ -1,-1 };
            if (!game_over && user != turn)
            {
                if (!aiTurn) { aiTurn = true; aiTimer = GetTime(); }
                else if (GetTime() - aiTimer >= 0.5)
                {
                    auto best = optimalAction(board);     // returns {score, Move}
                    if (best.second != NO_MOVE)
                        board = result(board, best.second);
                    aiTurn = false;
                }
            }

            // Human move
            if (!game_over && user == turn && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 m = GetMousePosition();
                for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j)
                {
                    if (board[i][j] == EMPTY && CheckCollisionPointRec(m, tiles[i][j]))
                    {
                        board = result(board, { i, j });
                    }
                }
            }

            // Play Again (must be outside the !game_over branch)
            if (game_over)
            {
                Rectangle again{ WIDTH / 3.0f, HEIGHT - 65.0f, WIDTH / 3.0f, 50.0f };
                DrawRectangleRec(again, RAYWHITE);
                const char* text = "Play Again";
                int twA = MeasureText(text, 24);
                DrawText(text, (int)(again.x + (again.width - twA) / 2),
                               (int)(again.y + (again.height - 24) / 2), 24, BLACK);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    Vector2 mp = GetMousePosition();
                    if (CheckCollisionPointRec(mp, again))
                    {
                        user = 0;
                        board = initialState();
                        aiTurn = false;
                    }
                }
            }
        }

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}
	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
