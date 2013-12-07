#include "menu.h"

#include "game.h"

Menu::Menu()
{
    text = unique_ptr<Text> (new Text);
}

Menu::~Menu()
{

}

void Menu::draw()
{
    string menuname, centertext;
    switch (engine.gameState) {
    case GAME_START_MENU:
        menuname = "Asteroid Racing";
        centertext = "Начать новую игру.";
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
            ast->reset ();
            ship->reset ();
            for (int i = 0; i < rand ()%20 + 2; ++i)
                ast->addAsteroid (ballRand (200.f), linearRand (2.f, 25.f));
        case GAME_PAUSE_MENU:
            engine.gameState = GAME_PLAYING;
            break;
        case GAME_WIN_MENU:
        case GAME_OVER_MENU:
            engine.gameState = GAME_START_MENU;
            break;
        }
    }
}
