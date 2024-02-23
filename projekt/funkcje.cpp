#include "naglowek.h"

void drawButton(Font font, Rectangle button, Color color, const char* tekst) {
    DrawRectangleRec(button, color);
    DrawRectangleLinesEx(button, 2, BLACK);
    DrawTextEx(font, tekst, { button.x + 55, button.y + 10 }, 40, 2, BLACK);
}

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