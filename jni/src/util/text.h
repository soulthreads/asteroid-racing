#ifndef TEXT_H
#define TEXT_H

#include "engine.h"

enum alignment { A_MINUS, A_CENTER, A_PLUS };

struct textUnit {
    vec2 position;
//    vec4 color;
    float size;
    alignment ax, ay;
    string text;
};

class Text
{
private:
    GLuint program;

    GLuint u_MvpMatrixHandle;
    GLuint u_TexUnitHandle;

    GLuint a_PositionHandle;
    GLuint a_TexCoordsHandle;
//    GLuint a_ColorHandle;

    vector<GLfloat> vertexData;

    GLuint stride;

    map<string, textUnit> strings;

    GLuint fontTex;
    static const float w = 1/16.0, h = 1/8.0;
    static const int roww = 1/w;
    vector<GLfloat> makeSymbol (vec2 pos, float size, char c);

public:
    Text ();
    ~Text ();
    void draw (const Engine &engine);
    void addText (const string& key, const textUnit& text);
};

#endif // TEXT_H
