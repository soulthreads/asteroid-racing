#ifndef GAME_H
#define GAME_H

#include "util/text.h"
#include "objects/asteroids.h"
#include "objects/ship.h"

extern unique_ptr<Text> text;
extern unique_ptr<Asteroids> ast;
extern unique_ptr<Ship> ship;

#endif // GAME_H
