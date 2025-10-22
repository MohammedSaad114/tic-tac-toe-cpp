#include "app.h"
#include <limits>

static const Move kNoMove{ -1,-1 };

void initApp(App& a, int w, int h)
{
    a.width = w; a.height = h;
    a.state = State::Menu;
    a.user = 0;
    a.board = initialState();
    a.aiTurn = false;
    a.aiTimer = 0.0;
    a.aiDelay = 0.5;
    a.tileSize = 80;
    a.origin = { a.width / 2.0f - 1.5f * a.tileSize, a.height / 2.0f - 1.5f * a.tileSize };
    a.uiFont = LoadFontEx("resources/OpenSans_Condensed-Bold.ttf", 128, nullptr, 0);
    SetTextureFilter(a.uiFont.texture, TEXTURE_FILTER_BILINEAR);
}

static void drawCenteredText(App& a, const char* s, Rectangle r, float fontSize, Color c)
{
   
    Vector2 size = MeasureTextEx(a.uiFont, s, fontSize, 1.0f);
    Vector2 pos{
       r.x + (r.width - size.x) / 2.0f,
       r.y + (r.height - size.y) / 2.0f
    };
    DrawTextEx(a.uiFont, s, pos, fontSize, 1.0f, c);
}

static void drawMenu(App& a)
{
    // Title
    const int tfs = 40;
    int tw = MeasureText("Play Tic-Tac-Toe", tfs);
    DrawText("Play Tic-Tac-Toe", (a.width - tw) / 2, 40, tfs, RAYWHITE);

    // Buttons
    Rectangle bx{ a.width / 8.0f,   a.height / 2.0f, a.width / 4.0f, 50.0f };
    Rectangle bo{ 5 * a.width / 8.0f, a.height / 2.0f, a.width / 4.0f, 50.0f };

    DrawRectangleRec(bx, RAYWHITE);
    int twX = MeasureText("Play as X", 24);
    DrawText("Play as X", bx.x + (bx.width - twX) / 2, bx.y + (bx.height - 24) / 2, 24, BLACK);

    DrawRectangleRec(bo, RAYWHITE);
    int twO = MeasureText("Play as O", 24);
    DrawText("Play as O", bo.x + (bo.width - twO) / 2, bo.y + (bo.height - 24) / 2, 24, BLACK);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 m = GetMousePosition();
        if (CheckCollisionPointRec(m, bx)) { a.user = X; a.board = initialState(); a.state = State::Playing; }
        else if (CheckCollisionPointRec(m, bo)) { a.user = O; a.board = initialState(); a.state = State::Playing; }
    }
}

static void drawBoard(App& a)
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            Rectangle r{ a.origin.x + j * a.tileSize, a.origin.y + i * a.tileSize,
                         (float)a.tileSize, (float)a.tileSize };
            a.tiles[i][j] = r;
            DrawRectangleLinesEx(r, 3.0f, RAYWHITE);

            int v = a.board[i][j];
            if (v != EMPTY)
            {
                const char* s = (v == X) ? "X" : "O";
                drawCenteredText(a, s, a.tiles[i][j], 60.0f, RAYWHITE);
            }
        }
}

static void playAgain(App& a)
{
    Rectangle again{ a.width / 3.0f, a.height - 65.0f, a.width / 3.0f, 50.0f };
    DrawRectangleRec(again, RAYWHITE);
    int tw = MeasureText("Play Again", 24);
    DrawText("Play Again", again.x + (again.width - tw) / 2, again.y + (again.height - 24) / 2, 24, BLACK);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 m = GetMousePosition();
        if (CheckCollisionPointRec(m, again))
        {
            a.state = State::Menu;
            a.user = 0;
            a.board = initialState();
            a.aiTurn = false;
        }
    }
}

static void updatePlaying(App& a)
{
    bool over = terminal(a.board);
    int turn = player(a.board);

    // Status
    const char* status = nullptr;
    if (over)
    {
        int w = winner(a.board);
        status = (w == 0) ? "Game Over: Tie." : (w == X ? "Game Over: X wins." : "Game Over: O wins.");
    }
    else
    {
        status = (a.user == turn) ? ((a.user == X) ? "Play as X" : "Play as O")
            : "Computer thinking...";
    }
    const int sfs = 32;
    int sw = MeasureText(status, sfs);
    DrawText(status, (a.width - sw) / 2, 20, sfs, RAYWHITE);

    // Board
    drawBoard(a);

    // AI move
    if (!over && a.user != turn)
    {
        if (!a.aiTurn) { a.aiTurn = true; a.aiTimer = GetTime(); }
        else if (GetTime() - a.aiTimer >= a.aiDelay)
        {
            auto best = optimalAction(a.board);        // {score, Move}
            if (best.second != kNoMove) a.board = result(a.board, best.second);
            a.aiTurn = false;
        }
    }

    // Human move
    if (!over && a.user == turn && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 m = GetMousePosition();
        for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j)
        {
            if (a.board[i][j] == EMPTY && CheckCollisionPointRec(m, a.tiles[i][j]))
            {
                a.board = result(a.board, { i,j });
            }
        }
    }

    // Play again UI
    if (over) playAgain(a);
}

void frame(App& app)
{
    switch (app.state)
    {
        case State::Menu:    drawMenu(app);    break;
        case State::Playing: updatePlaying(app); break;
    }
}

void cleanupApp(App& app) {
    UnloadFont(app.uiFont);
}
