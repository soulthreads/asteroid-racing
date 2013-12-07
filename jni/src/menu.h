#ifndef MENU_H
#define MENU_H

#include <android_native_app_glue.h>
#include "engine.h"
#include "game.h"
#include "gui/layout.h"

class Menu
{
private:
    unique_ptr<Layout> startLayout;
public:
    Menu ();
    ~Menu ();
    void draw ();
    void handleTouch (int actionMasked, float x, float y);
};

#endif // MENU_H
