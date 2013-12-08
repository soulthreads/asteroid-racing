#ifndef GUI_H
#define GUI_H

#include "game.h"
#include "button.h"


class Layout
{
private:
    GLuint program;

    GLuint u_MvpMatrixHandle;

    GLuint a_PositionHandle;
    GLuint a_ColorHandle;

    vector<unique_ptr<Element>> elements;
    vector<GLfloat> vertices;

    int token;
    void init ();
public:
    Layout();
    void draw ();
    void touchDown(float x, float y);
    void addButton(const string &label, Rect rect, vec4 bgColor, vec4 fgColor, Functor f);
};

#endif // GUI_H
