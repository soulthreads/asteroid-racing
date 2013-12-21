#ifndef ELEMENT_H
#define ELEMENT_H

#include "engine.h"
#include "util/text.h"

struct Rect {
    float x, y, w, h;
    Rect (float _x, float _y, float _w, float _h)
        : x(_x), y(_y), w(_w), h(_h) {}
};

class Element
{
protected:
    Rect r;
    vec4 bg, fg;

    vector<GLfloat> vertices;
    virtual void updateVertices ();

    string id;
    function<void()> callback;

    bool scrollable = false;

public:
    Element(Rect rect, vec4 bgColor, vec4 fgColor, function<void()> f);
    virtual ~Element () {}
    string &getId ();
    void setId (string newId);
    virtual vector<textUnit> getTexts () = 0;
    virtual vector<GLfloat> &getVertices();
    Rect getRect() const;
    void setRect(const Rect &value);
    vec4 getBgColor() const;
    void setBgColor(const vec4 &value);
    vec4 getFgColor() const;
    void setFgColor(const vec4 &value);
    bool isScrollable ();

    virtual void run (float x, float y);

    virtual void move (float dx, float dy) {}

    static const int components = 2 + 4;
    static const int stride = components * sizeof (GLfloat);
};

#endif // ELEMENT_H
