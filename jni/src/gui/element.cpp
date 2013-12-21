#include "element.h"


Rect Element::getRect() const
{
    return r;
}

void Element::setRect(const Rect &value)
{
    r = value;
    updateVertices ();
}

vec4 Element::getBgColor() const
{
    return bg;
}

void Element::setBgColor(const vec4 &value)
{
    bg = value;
    updateVertices ();
}

vec4 Element::getFgColor() const
{
    return fg;
}

void Element::setFgColor(const vec4 &value)
{
    fg = value;
}

bool Element::isScrollable()
{
    return scrollable;
}

void Element::run(float x, float y)
{
    callback ();
}

vector<GLfloat> &Element::getVertices()
{
    return vertices;
}

void Element::updateVertices()
{
    vertices.clear ();
    vertices = {
        r.x,     r.y,     bg[0], bg[1], bg[2], bg[3],
        r.x,     r.y-r.h, bg[0], bg[1], bg[2], bg[3],
        r.x+r.w, r.y,     bg[0], bg[1], bg[2], bg[3],
        r.x,     r.y-r.h, bg[0], bg[1], bg[2], bg[3],
        r.x+r.w, r.y-r.h, bg[0], bg[1], bg[2], bg[3],
        r.x+r.w, r.y,     bg[0], bg[1], bg[2], bg[3]
    };
}

Element::Element(Rect rect, vec4 bgColor, vec4 fgColor, function<void ()> f)
    : r (rect), bg (bgColor), fg (fgColor), callback (f)
{
    updateVertices ();
}

string &Element::getId()
{
    return id;
}

void Element::setId (string newId) {
    id = newId;
}
