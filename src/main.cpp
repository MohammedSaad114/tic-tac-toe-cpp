/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "game.h"
#include "app.h"

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	
	// Create the window and OpenGL context
	const int WIDTH = 600;
	const int HEIGHT = 400;
	InitWindow(WIDTH, HEIGHT, "Tic Tac Toe");
	SetTargetFPS(60);

	App app;
	initApp(app, WIDTH, HEIGHT);
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();
		ClearBackground(BLACK);

		frame(app);

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}
	cleanupApp(app);
	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
