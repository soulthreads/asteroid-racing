#ifndef MENU_H
#define MENU_H

#include "engine.h"
#include "util/text.h"

class Menu
{
private:
    unique_ptr<Text> text;
public:
    Menu ();
    ~Menu ();
    void draw (Engine &engine);
    void handleTouch (Engine &engine, float x, float y);
};

#endif // MENU_H
