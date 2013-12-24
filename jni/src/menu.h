#ifndef MENU_H
#define MENU_H

#include <android_native_app_glue.h>
#include "engine.h"
#include "game.h"

class Layout;

class Menu
{
private:
    vector <Layout> layouts;
    GameState prevState;
    void gameStart(int index);
public:
    Menu ();
    ~Menu ();
    void draw ();
    void handleTouch (int actionMasked, float x, float y);
    void setState (GameState state);
    int handleKeyPress(int keyCode);
};

#endif // MENU_H
