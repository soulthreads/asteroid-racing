#include "menu.h"

Menu::Menu()
{
    text = unique_ptr<Text> (new Text);
    text->addText ("start", textUnit {vec2(0,0), 1, A_CENTER, A_CENTER, "Tap anywhere to start."});
}

Menu::~Menu()
{

}

void Menu::draw(Engine &engine)
{
    string menuname;
    switch (engine.gameState) {
    case GAME_START_MENU:
        menuname = "Main Menu";
        break;
    case GAME_PAUSE_MENU:
        menuname = "Pause Menu";
        break;
    case GAME_WIN_MENU:
        menuname = "You won!";
        break;
    default:
        menuname = "Something other";
    }
    text->addText ("menuname", textUnit {vec2 (0, 1), 2, A_CENTER, A_PLUS, menuname});

    text->draw (engine);
}

void Menu::handleTouch(Engine &engine, float x, float y)
{
    switch (engine.gameState) {
    case GAME_START_MENU:
    case GAME_PAUSE_MENU:
        engine.gameState = GAME_PLAYING;
        break;
    }

}
