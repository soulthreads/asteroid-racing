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
};

#endif // GUI_H
