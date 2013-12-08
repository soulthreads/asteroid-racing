#ifndef GUI_H
#define GUI_H

#include "game.h"
#include "button.h"
#include "list.h"

class Layout
{
private:
    GLuint program;

    GLuint u_MvpMatrixHandle;

    GLuint a_PositionHandle;
    GLuint a_ColorHandle;

    vector<unique_ptr<Element>> elements;
    vector<GLfloat> vertices;

    textUnit name;
    int touchIndex = -1;
    float px, py;

    int token;
    void init ();

public:
    Layout();
    void draw ();
    void addButton(const string &label, Rect rect, vec4 bgColor, vec4 fgColor, Functor f = []{});
    void setName (const string layoutName);
    void touchDown(float x, float y);
    void touchMove(float x, float y);
    void touchUp(float x, float y);
    void addList(const string name, const vector<string> listElements, Rect rect, vec4 bgColor, vec4 fgColor, Functor f = []{});
    Element *getById(const string id);
};

#endif // GUI_H
