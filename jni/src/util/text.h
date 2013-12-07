#ifndef TEXT_H
#define TEXT_H

#include "engine.h"

enum alignment { A_MINUS, A_CENTER, A_PLUS };

struct textUnit {
    vec2 position;
    vec4 color;
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
    GLuint u_ColorsHandle;

    GLuint a_PositionHandle;
    GLuint a_TexCoordsHandle;
    GLuint a_ColorHandle;

    GLuint components;

    GLuint stride;

    map<string, textUnit> strings;
    vector<GLfloat> vertices;
    vector<GLfloat> colors;

    GLuint fontTex;
    static const float texw = 512, texh = 256;
    static const float symw = 16, symh = 32;
    static const int rowc = texw / symw;
    static const float factor = 1/256.0;
    vector<GLfloat> makeSymbol (vec2 pos, float colorIndex, float size, uint ch);

    bool changed;
    void updateVertexData();

    int token;
    void init();

public:
    Text ();
    ~Text ();
    void draw ();
    void addText (const string& key, const textUnit& text);
    void deleteText(const string &key);
    void reset();
};

#endif // TEXT_H
