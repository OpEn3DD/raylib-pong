#include <iostream>
#include <raylib.h>
#include "naglowek.h"

using namespace std;

//punkty
int player1_score = 0;
int player2_score = 0;

class Ball {
public:
    int radius, speed_x, speed_y;
    float pos_x, pos_y;

    void Draw() {
        DrawCircle(pos_x, pos_y, radius, WHITE);
    }

    void Update() {
        pos_x += speed_x;
        pos_y += speed_y;

        if (pos_x - radius <= 0) {
            player2_score++;
            resetBall();
        }
        if (pos_x + radius >= GetScreenWidth()) {
            player1_score++;
            resetBall();
        }
        if (pos_y - radius <= 0 || pos_y + radius >= GetScreenHeight()) speed_y *= -1;
    }

    void resetBall() {
        pos_x = GetScreenWidth() / 2;
        pos_y = GetScreenHeight() / 2;
        int values[2] = { -1, 1 };
        speed_x *= values[GetRandomValue(0, 1)];
        speed_y *= values[GetRandomValue(0, 1)];
    }

    Ball(int r, int sx, int sy, float px, float py) {
        radius = r;
        speed_x = sx;
        speed_y = sy;
        pos_x = px;
        pos_y = py;
    }

};

class Paddle {
public:
    float pos_x, pos_y, height, speed;

    void Draw() {
        DrawRectangle(pos_x, pos_y, 25, height, WHITE);
    }

protected:
    void Check() {
        if (pos_y <= 0)
            pos_y = 0;

        if (pos_y + height >= GetScreenHeight())
            pos_y = GetScreenHeight() - height;
    }
};

class Paddle1 : public Paddle {
public:
    void Update() {
        if (IsKeyDown(KEY_W))
            pos_y -= speed;

        if (IsKeyDown(KEY_S))
            pos_y += speed;

        Check();
    }

    Paddle1(int px, int py, int h, float s) {
        pos_x = px;
        pos_y = py;
        height = h;
        speed = s;
    }
};

class Paddle2 : public Paddle {
public:
    void Update() {
        if (IsKeyDown(KEY_UP))
            pos_y -= speed;

        if (IsKeyDown(KEY_DOWN))
            pos_y += speed;

        Check();
    }

    Paddle2(int px, int py, int h, float s) {
        pos_x = px;
        pos_y = py;
        height = h;
        speed = s;
    }
};

class PowerUp {
public:
    float pos_x, pos_y, radius;

    void Draw() {
        DrawCircle(pos_x, pos_y, radius, WHITE);
    }

    void Update() {

    }

    PowerUp(int x, int y, int r) {
        pos_x = x;
        pos_y = y;
        radius = r;
    }
};

//Funkcje do licznika czasu, timer
//struct Timer {
//    double startTime;   // Start time (seconds)
//    double lifeTime;    // Lifetime (seconds)
//};
//
//
//void StartTimer(Timer* timer, double lifetime)
//{
//    timer->startTime = GetTime();
//    timer->lifeTime = lifetime;
//}
//
//bool TimerDone(Timer timer)
//{
//    return GetTime() - timer.startTime >= timer.lifeTime;
//}
//
//double GetElapsed(Timer timer)
//{
//    return GetTime() - timer.startTime;
//}

int main() {

    //ustawienia okna
    const int screenWidth = 1600;
    const int screenHeight = 800;

    //ustawienia przyciskow
    const int buttonWidth = 350;
    const int buttonHeight = 60;

    //timer
    //Timer powerUpTimer = { 0 };
    //double puDur = 5.0;

    //obiekty
    Ball ball(15, 6, 6, screenWidth / 2, screenHeight / 2);
    Paddle1 paddle1(0, screenHeight / 2, 150, 7);
    Paddle2 paddle2(screenWidth - 40, screenHeight / 2, 150, 7);
    //PowerUp powerUp1(GetRandomValue(200, screenWidth - 100), GetRandomValue(200, screenHeight - 100), 150);

    //przyciski
    Rectangle startButton = { screenWidth / 2 - buttonWidth / 2, 200, buttonWidth, buttonHeight };
    Rectangle instructionButton = { screenWidth / 2 - buttonWidth / 2, 300, buttonWidth, buttonHeight };
    Rectangle settingsButton = { screenWidth / 2 - buttonWidth / 2, 400, buttonWidth, buttonHeight };
    Rectangle returnButton = { screenWidth / 2 - (buttonWidth-90) / 2, 700, buttonWidth - 90, buttonHeight };

    //czcionka
    Font font = LoadFont("../fonts/Roboto.tff");

    //bool stan
    bool inGame = false;
    bool inSettings = false;
    bool inInstruction = false;
    bool endOfGame = false;
    bool p1w = false;

    //inicjalizacja okna gry
    InitWindow(screenWidth, screenHeight, "Ping Pong game.");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //sprawdzamy aktualny stan 
        if (!inGame && !inSettings && !inInstruction && !endOfGame) {
            drawButton(font, startButton, GRAY, "START GRY");
            drawButton(font, instructionButton, GRAY, "INSTRUKCJA");
            drawButton(font, settingsButton, GRAY, "USTAWIENIA");

            //jezeli kliknieto start stan zmienia sie na w grze
            if (CheckCollisionPointRec(GetMousePosition(), startButton)) {

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) inGame = true;
                drawButton(font, startButton, LIGHTGRAY, "START GRY");
            }

            //jezeli kliknieto instrukcja stan zmienia sie na w instrukcji
            if (CheckCollisionPointRec(GetMousePosition(), instructionButton)) {
                
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))  inInstruction = true;
                drawButton(font, instructionButton, LIGHTGRAY, "INSTRUKCJA");
            }

            //jezeli kliknieto ustawienia stan zmienia sie na w ustawieniach
            if (CheckCollisionPointRec(GetMousePosition(), settingsButton)) {
                
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) inSettings = true;
                drawButton(font, settingsButton, LIGHTGRAY, "USTAWIENIA");

            }
        }
        else if(inGame){     
            //update
            ball.Update();
            paddle1.Update();
            paddle2.Update();


            //sprawdzamy kolizje z paletkami
            if (CheckCollisionCircleRec(Vector2{ ball.pos_x, ball.pos_y }, 25, Rectangle{ paddle1.pos_x, paddle1.pos_y, 25, paddle1.height })) ball.speed_x *= -1;
            if (CheckCollisionCircleRec(Vector2{ ball.pos_x, ball.pos_y }, 25, Rectangle{ paddle2.pos_x, paddle2.pos_y, 25, paddle2.height })) ball.speed_x *= -1;

            ////kolizja z powerup
            //if (CheckCollisionCircles(Vector2{ ball.pos_x, ball.pos_y }, 25, Vector2{ powerUp1.pos_x, powerUp1.pos_y }, 150)) {
            //    StartTimer(&powerUpTimer, puDur);
            //    powerUp1.radius = 0;
            //    ball.speed_x = 3;
            //}

            ////sprawdzanie konca odliczania, timer
            //if (!TimerDone(powerUpTimer)) {
            //    ball.speed_x = 6;
            //}

            //drawing
            ClearBackground(BLACK);
            ball.Draw();
            paddle1.Draw();
            paddle2.Draw();
            //powerUp1.Draw();

            //rysowanie aktualnego wyniku
            DrawText(TextFormat("%i", player1_score), screenWidth / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player2_score), 3* screenWidth / 4 - 20, 20, 80, WHITE);

            //sprawdzanie konca gry
            if (player1_score == 3) {
                inGame = false;
                endOfGame = true;
            }

            if (player2_score == 3) {
                inGame = false;
                endOfGame = true;
                p1w = true;
            }
        }
        else if (inInstruction) {
            //ekran instrukcji
            DrawText("Witaj w grze Pong!", screenWidth / 2 - MeasureText("Witaj w grze Pong!", 40) / 2, screenHeight / 2 - 140, 40, BLACK);
            DrawText("Sterowanie:", screenWidth / 2 - MeasureText("Sterowanie:", 40) / 2, screenHeight / 2 - 95, 40, BLACK);
            DrawText("Gracz 1: W i S", screenWidth / 2 - MeasureText("Gracz 1: W i S", 40) / 2, screenHeight / 2 - 50, 40, BLACK);
            DrawText("Gracz 2: Strzalki gora i dol", screenWidth / 2 - MeasureText("Gracz 2: Strzalki gora i dol", 40) / 2, screenHeight / 2 - 5, 40, BLACK);
            DrawText("Gra konczy sie po zdobyciu 3 punktow przez jednego z graczy.", screenWidth / 2 - MeasureText("Gra konczy sie po zdobyciu 3 punktow przez jednego z graczy.", 40) / 2, screenHeight / 2 + 40, 40, BLACK);

            //przycisk powrot
            drawButton(font, returnButton, GRAY, "POWROT");
            if (CheckCollisionPointRec(GetMousePosition(), returnButton)) {

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) inInstruction = false;
                drawButton(font, returnButton, LIGHTGRAY, "POWROT");
            }
        }
        else if (inSettings) {

            //przycisk powrot
            drawButton(font, returnButton, GRAY, "POWROT");
            if (CheckCollisionPointRec(GetMousePosition(), returnButton)) {

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) inSettings = false;
                drawButton(font, returnButton, LIGHTGRAY, "POWROT");
            }
        }
        else if (endOfGame) {
            //wyswietlenie ekranu konca gry
            ClearBackground(RAYWHITE);
            if(p1w) 
                DrawText("Gratulacje, wygral gracz 2 !", screenWidth / 2 - MeasureText("Gratulacje, wygral gracz 2 !", 40) / 2, screenHeight / 2 - 60, 40, BLACK);
            else
                DrawText("Gratulacje, wygral gracz 1 !", screenWidth / 2 - MeasureText("Gratulacje, wygral gracz 1 !", 40) / 2, screenHeight / 2 - 60, 40, BLACK);

            //przycisk powrot
            drawButton(font, returnButton, GRAY, "POWROT");
            if (CheckCollisionPointRec(GetMousePosition(), returnButton)) {

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) endOfGame = false;
                drawButton(font, returnButton, LIGHTGRAY, "POWROT");
                p1w = false;
                player1_score = 0;
                player2_score = 0;
            }

        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}