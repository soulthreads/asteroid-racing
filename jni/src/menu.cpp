#include "menu.h"

Menu::Menu(Engine &engine)
{
    text = unique_ptr<Text> (new Text);
}

Menu::~Menu()
{

}

void Menu::draw(Engine &engine)
{
    string menuname, centertext;
    switch (engine.gameState) {
    case GAME_START_MENU:
        menuname = "Asteroid Racing";
        centertext = "Нажмите в любое место экрана, чтобы начать.";
        break;
    case GAME_PAUSE_MENU:
        menuname = "Пауза";
        centertext = "Нажмите в любое место экрана, чтобы продолжить.";
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
    text->addText ("menuname", textUnit {vec2 (0, 1), 2, A_CENTER, A_PLUS, menuname});
    text->addText ("start", textUnit {vec2(0,0), 1, A_CENTER, A_CENTER, centertext});

    text->draw (engine);
}

void Menu::handleTouch(Engine &engine, int actionMasked, float x, float y)
{
    switch (engine.gameState) {
    case GAME_START_MENU:
    case GAME_PAUSE_MENU:
        if (actionMasked == AMOTION_EVENT_ACTION_DOWN)
            engine.gameState = GAME_PLAYING;
        break;
    case GAME_WIN_MENU:
    case GAME_OVER_MENU:
        if (actionMasked == AMOTION_EVENT_ACTION_DOWN)
            engine.gameState = GAME_START_MENU;
        break;
    }

}
