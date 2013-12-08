#include "menu.h"

#include "game.h"

void gameStart () {
    engine.gameState = GAME_PLAYING;
    text->reset ();
    ast->reset ();
    ship->reset ();
    for (int i = 0; i < rand ()%20 + 2; ++i)
        ast->addAsteroid (ballRand (200.f), linearRand (2.f, 25.f));
}

Menu::Menu()
{
    layouts = vector<Layout> (GAME_PLAYING); // All menus before playing state

    layouts[GAME_START_MENU].setName ("Asteroid Racing");
    layouts[GAME_START_MENU].addButton ("Start",
                                        Rect (-0.5, 0.3, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){gameStart ();});
    layouts[GAME_START_MENU].addButton ("Stats",
                                        Rect (-0.5, 0.0, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){setState (GAME_STATS_MENU);});

    layouts[GAME_START_MENU].addButton ("Exit",
                                        Rect (-0.5, -0.3, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){engine.exitFlag = true;});

    layouts[GAME_PAUSE_MENU].setName ("Pause");
    layouts[GAME_PAUSE_MENU].addButton ("Resume",
                                        Rect (-0.5, 0.15, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){setState (GAME_PLAYING); text->reset ();});

    layouts[GAME_PAUSE_MENU].addButton ("Main Menu",
                                        Rect (-0.5, -0.15, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){setState (GAME_START_MENU);});

    layouts[GAME_WIN_MENU].setName ("Win");
    layouts[GAME_WIN_MENU].addButton ("Next level",
                                        Rect (-0.5, 0.3, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){gameStart ();});

    layouts[GAME_WIN_MENU].addButton ("Stats",
                                        Rect (-0.5, 0.0, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){setState (GAME_STATS_MENU);});

    layouts[GAME_WIN_MENU].addButton ("Main Menu",
                                        Rect (-0.5, -0.3, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){setState (GAME_START_MENU);});

    layouts[GAME_OVER_MENU].setName ("Game over");
    layouts[GAME_OVER_MENU].addButton ("Restart",
                                        Rect (-0.5, 0.3, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){gameStart ();});

    layouts[GAME_OVER_MENU].addButton ("Stats",
                                        Rect (-0.5, 0.0, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){setState (GAME_STATS_MENU);});

    layouts[GAME_OVER_MENU].addButton ("Main Menu",
                                        Rect (-0.5, -0.3, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){setState (GAME_START_MENU);});

    layouts[GAME_STATS_MENU].setName ("Stats");
}

Menu::~Menu()
{

}

void Menu::draw()
{
    text->reset ();
    layouts[engine.gameState].draw ();
    text->draw ();
}

void Menu::handleTouch(int actionMasked, float x, float y)
{
    switch (actionMasked) {
    case AMOTION_EVENT_ACTION_DOWN:
        layouts[engine.gameState].touchDown (x, y);
        break;
    case AMOTION_EVENT_ACTION_UP:
    case AMOTION_EVENT_ACTION_POINTER_UP:
        layouts[engine.gameState].touchUp (x, y);
        break;
    default:
        break;
    }
}

int Menu::handleKeyPress (int keyCode) {
    switch (engine.gameState) {
    case GAME_PAUSE_MENU:
        if ((keyCode == AKEYCODE_MENU) || (keyCode == AKEYCODE_BACK)) {
            setState (GAME_PLAYING);
            text->reset ();
            return 1;
        }
    case GAME_WIN_MENU:
    case GAME_OVER_MENU:
        if (keyCode == AKEYCODE_BACK) {
            setState (GAME_START_MENU);
            return 1;
        }
    case GAME_STATS_MENU:
        if (keyCode == AKEYCODE_BACK) {
            setState (prevState);
            return 1;
        }
    default:
        return 0;
    }
}

void Menu::setState(GameState state)
{
    prevState = engine.gameState;
    engine.gameState = state;
}
