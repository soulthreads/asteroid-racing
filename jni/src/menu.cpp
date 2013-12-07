#include "menu.h"

#include "game.h"

Menu::Menu()
{
    startLayout = unique_ptr<Layout> (new Layout);
}

Menu::~Menu()
{

}

void Menu::draw()
{
    text->reset ();

    string menuname, centertext;
    switch (engine.gameState) {
    case GAME_START_MENU:
        startLayout->draw ();
        menuname = "Asteroid Racing";
        centertext = "";
        break;
    case GAME_PAUSE_MENU:
        menuname = "Пауза";
        centertext = "Продолжить игру.";
        break;
    case GAME_WIN_MENU:
        menuname = "Win";
        centertext = "Вернуться в главное меню.";
        break;
    case GAME_OVER_MENU:
        menuname = "Game over";
        centertext = "Вернуться в главное меню.";
        break;
    case GAME_STATS_MENU:
        menuname = "Stats";
        centertext = "Not implemented yet";
        break;
    default:
        menuname = "Something other";
    }
    text->addText ("menuname", textUnit {vec2 (0, 1),
                                         vec4 (1),
                                         2, A_CENTER, A_PLUS, menuname});
    text->addText ("start", textUnit {vec2(0,0),
                                      vec4 (1),
                                      1, A_CENTER, A_CENTER, centertext});

    text->draw ();
}

void Menu::handleTouch(int actionMasked, float x, float y)
{
    if (actionMasked == AMOTION_EVENT_ACTION_DOWN) {
        switch (engine.gameState) {
        case GAME_START_MENU:
            startLayout->touchDown (x, y);
            break;
        case GAME_PAUSE_MENU:
            engine.gameState = GAME_PLAYING;
            text->reset ();
            break;
        case GAME_WIN_MENU:
        case GAME_OVER_MENU:
        case GAME_STATS_MENU:
            engine.gameState = GAME_START_MENU;
            break;
        }
    }
}
