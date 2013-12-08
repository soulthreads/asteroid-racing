#include "menu.h"

#include "game.h"

Menu::Menu()
{
    startLayout = unique_ptr<Layout> (new Layout);
    startLayout->addButton ("Start",
                            Rect (-0.5, 0.3, 1, 0.2),
                            vec4 (0.5), vec4 (1),
                            [&]()
    {
        engine.gameState = GAME_PLAYING;
        text->reset ();
        ast->reset ();
        ship->reset ();
        for (int i = 0; i < rand ()%20 + 2; ++i)
            ast->addAsteroid (ballRand (200.f), linearRand (2.f, 25.f));
    });

    startLayout->addButton ("Stats",
                            Rect (-0.5, 0.0, 1, 0.2),
                            vec4 (0.5), vec4 (1),
                            [&](){engine.gameState = GAME_STATS_MENU;});

    startLayout->addButton ("Exit",
                            Rect (-0.5, -0.3, 1, 0.2),
                            vec4 (0.5), vec4 (1),
                            [&](){engine.exitFlag = true;});
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
        default:
            engine.gameState = GAME_START_MENU;
            break;
        }
    }
}
