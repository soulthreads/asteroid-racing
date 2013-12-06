#ifndef MENU_H
#define MENU_H

#include "engine.h"
#include "util/text.h"
#include <android_native_app_glue.h>

class Menu
{
private:
    unique_ptr<Text> text;
public:
    Menu (Engine &engine);
    ~Menu ();
    void draw (Engine &engine);
    void handleTouch (Engine &engine, int actionMasked, float x, float y);
};

#endif // MENU_H
