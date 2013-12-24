#include "levels.h"
#include <sstream>

using namespace std;

Levels::Levels()
{
    vector<string> lfiles = listFilesInDir ("levels");
    for (auto &f: lfiles) {
        auto asset = getAssetData ("levels/"+f);
        level l;
        istringstream iss (asset.data);
        getline (iss, l.name);
        iss >> l.time;
        string line;
        char c;
        float x, y, z, r, qa, qx, qy, qz;
        while (getline (iss, line)) {
            istringstream lstr (line);
            lstr >> c;
            switch (c) {
            case 's':
                lstr >> x >> y >> z >> qa >> qx >> qy >> qz;
                l.start = vec3 {x, y, z};
                l.startq = angleAxis (qa, vec3 {qx, qy, qz});
                break;
            case 'f':
                lstr >> x >> y >> z >> qa >> qx >> qy >> qz;
                l.finish = vec3 {x, y, z};
                l.finishq = angleAxis (qa, vec3 {qx, qy, qz});
                break;
            case 'a':
                lstr >> x >> y >> z >> r;
                l.asteroids.push_back (vec4 {x, y, z, r});
            }
        }
        levels.push_back (l);
    }
}

size_t Levels::size()
{
    return levels.size ();
}

vector<string> Levels::getLevelNames()
{
    vector<string> ret;
    for (auto &l : levels) {
        ret.push_back (l.name);
    }

    return ret;
}

vector<vec4>& Levels::getAsteroids(size_t index)
{
    return levels[index].asteroids;
}

float Levels::getTime(size_t index)
{
    return levels[index].time;
}

vec3 Levels::getStartPosition(size_t index)
{
    return levels[index].start;
}

quat Levels::getStartOrientation(size_t index)
{
    return levels[index].startq;
}

vec3 Levels::getFinishPosition(size_t index)
{
    return levels[index].finish;
}
