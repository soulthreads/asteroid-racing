#ifndef GAME_H
#define GAME_H

#include <memory>
#include <glm/core/type_vec4.hpp>

class Text;
class Asteroids;
class Ship;
class Timer;
class Particles;
class Levels;

extern std::unique_ptr<Text> text;
extern std::unique_ptr<Asteroids> ast;
extern std::unique_ptr<Ship> ship;
extern std::unique_ptr<Levels> levels;
extern size_t currentLevel;
extern std::unique_ptr<Timer> timer;
extern glm::vec3 endPos;
extern std::unique_ptr<Particles> finishParticles;

#endif // GAME_H
