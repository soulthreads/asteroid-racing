#include "menu.h"
#include "gui/element.h"
#include "gui/list.h"
#include "gui/layout.h"
#include "game.h"
#include "util/timer.h"
#include "util/levels.h"
#include "objects/asteroids.h"
#include "objects/ship.h"

#define LOG_TAG "AR_menu"
#include "util/logs.h"

Menu::Menu()
{
    layouts = vector<Layout> (GAME_PLAYING); // All menus before playing state

    layouts[GAME_START_MENU].setName ("Asteroid Racing");
    layouts[GAME_START_MENU].addButton ("Start",
                                        Rect (-0.5, 0.3, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){setState (GAME_SELECT_MENU);});
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
                                        [&](){setState (GAME_LOADING); text->reset ();});

    layouts[GAME_PAUSE_MENU].addButton ("Main Menu",
                                        Rect (-0.5, -0.15, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){setState (GAME_START_MENU);});

    layouts[GAME_WIN_MENU].setName ("Win");
    layouts[GAME_WIN_MENU].addButton ("Next level",
                                        Rect (-0.5, 0.3, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&]()
    {
        if (++currentLevel < levels->size ())
            gameStart (currentLevel);
    });

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
                                        [&](){gameStart (currentLevel);});

    layouts[GAME_OVER_MENU].addButton ("Stats",
                                        Rect (-0.5, 0.0, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){setState (GAME_STATS_MENU);});

    layouts[GAME_OVER_MENU].addButton ("Main Menu",
                                        Rect (-0.5, -0.3, 1, 0.2),
                                        vec4 (0.5), vec4 (1),
                                        [&](){setState (GAME_START_MENU);});

    levels = unique_ptr<Levels> (new Levels);
    layouts[GAME_SELECT_MENU].setName ("Select level");
    layouts[GAME_SELECT_MENU].addList ("levels", levels->getLevelNames (),
                                       Rect (-engine.aspectRatio+0.1, 0.7, 2, 1.6),
                                       vec4 (0.5), vec4 (1));
    layouts[GAME_SELECT_MENU].addButton ("Start",
                                         Rect (-engine.aspectRatio+2.2, 0.7,
                                               2*engine.aspectRatio-2.3, 0.2),
                                         vec4 (0.5), vec4 (1),
                                         [&]()
    {
        auto l = dynamic_cast<List&> (layouts[GAME_SELECT_MENU].getById ("levels"));
        currentLevel = l.getSelected ();
        gameStart (currentLevel);
    });
    layouts[GAME_SELECT_MENU].addButton ("Main Menu",
                                         Rect (-engine.aspectRatio+2.2, 0.4,
                                               2*engine.aspectRatio-2.3, 0.2),
                                         vec4 (0.5), vec4 (1),
                                         [&](){setState (GAME_START_MENU);});

    layouts[GAME_STATS_MENU].setName ("Stats");
    vector<string> stats {"Not implemented yet."};
    layouts[GAME_STATS_MENU].addList ("stats", stats,
                                      Rect (-engine.aspectRatio+0.1, 0.7, 2, 1.6),
                                      vec4 (0.5), vec4 (1));

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
    if (engine.gameState != GAME_PLAYING) {
        switch (actionMasked) {
        case AMOTION_EVENT_ACTION_DOWN:
            layouts[engine.gameState].touchDown (x, y);
            break;
        case AMOTION_EVENT_ACTION_MOVE:
            layouts[engine.gameState].touchMove (x, y);
            break;
        case AMOTION_EVENT_ACTION_UP:
        case AMOTION_EVENT_ACTION_POINTER_UP:
            layouts[engine.gameState].touchUp (x, y);
            break;
        default:
            break;
        }
    }
}

int Menu::handleKeyPress (int keyCode) {
    switch (engine.gameState) {
    case GAME_PAUSE_MENU:
        if ((keyCode == AKEYCODE_MENU) || (keyCode == AKEYCODE_BACK)) {
            setState (GAME_LOADING);
            text->reset ();
            return 1;
        }
    case GAME_WIN_MENU:
    case GAME_OVER_MENU:
    case GAME_SELECT_MENU:
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

void Menu::gameStart (int index) {
    engine.gameState = GAME_LOADING;
    engine.switchGameState = false;
    text->reset ();
    ast->reset ();
    ship->reset ();
    timer->setInitialTime (levels->getTime (index));
    for (auto &v : levels->getAsteroids (index)) {
        ast->addAsteroid (vec3(v), v.w);
    }
    endPos = levels->getFinishPosition (index);
}
