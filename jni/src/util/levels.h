#ifndef LEVELS_H
#define LEVELS_H

#include "engine.h"

class Levels
{
private:
    struct level {
        string name;
        float time;
        vec3 start;
        quat startq;
        vec3 finish;
        quat finishq;
        vector<vec4> asteroids;
    };
    vector<level> levels;

public:
    Levels();
    size_t size ();
    vector<string> getLevelNames ();
    vector<vec4> &getAsteroids(size_t index);
    float getTime (size_t index);
    vec3 getFinishPosition (size_t index);
};

#endif // LEVELS_H
