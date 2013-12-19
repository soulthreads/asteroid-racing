#include "list.h"

List::List (const vector<string> &listElements, Rect rect, vec4 bgColor, vec4 fgColor, Functor f = []{})
    : Element (rect, bgColor, fgColor, f)
{
    id = "list";
    elements = listElements;
    maxShift = elements.size () - (r.h)/elementHeight+2;
    scrollable = true;

    updateVertices ();
}

void List::setElements (const vector<string> &listElements) {
    elements = listElements;
}

vector<textUnit> List::getTexts()
{
    vector<textUnit> ret;

    float offset = elementHeight*3/2;
    for (int i = shift; (i < elements.size ()) && (offset < r.h-0.2); ++i) {
        ret.push_back (textUnit {vec2 (r.x+r.w/2.0, r.y-offset),
                                 fg, 1, A_CENTER, A_CENTER, elements[i]});
        offset += elementHeight;
    }

    return ret;
}

void List::run (float x, float y)
{
    if (!moved) {
        if ((y < r.y-r.h+elementHeight) && (shift < maxShift)) ++shift;
        else if ((y > r.y-elementHeight) && (shift > 0)) --shift;
        else selected = min (int (elements.size ()-1), max(0, int ((r.y-y) / elementHeight - 1 + shift)));
        updateVertices ();
        callback ();
    }

    fshift = 0;
    moved = false;
}

void List::move(float dx, float dy)
{
    fshift += dy / elementHeight;
    if (fabs (fshift) > 1) {
        shift = min (maxShift, max (0, shift+int (fshift)));
        fshift = 0;
        moved = true;
        updateVertices ();
    }
}

void List::updateVertices ()
{
    Element::updateVertices ();
    GLfloat xc = r.x+r.w/2;
    GLfloat yc1 = r.y-elementHeight/2.0;
    GLfloat yc2 = r.y-r.h+elementHeight/2.0;
    GLfloat u1 = shift > 0 ? 0.2 : -0.1;
    GLfloat u2 = (elements.size ()-shift) > r.h / elementHeight - 2 ? 0.2 : -0.1;

    vector<GLfloat> tris {
        xc,      yc1+0.05f, bg[0]+u1, bg[1]+u1, bg[2]+u1, bg[3],
        xc-0.1f, yc1-0.05f, bg[0]+u1, bg[1]+u1, bg[2]+u1, bg[3],
        xc+0.1f, yc1-0.05f, bg[0]+u1, bg[1]+u1, bg[2]+u1, bg[3],
        xc,      yc2-0.05f, bg[0]+u2, bg[1]+u2, bg[2]+u2, bg[3],
        xc+0.1f, yc2+0.05f, bg[0]+u2, bg[1]+u2, bg[2]+u2, bg[3],
        xc-0.1f, yc2+0.05f, bg[0]+u2, bg[1]+u2, bg[2]+u2, bg[3]
    };

    vertices.insert (end (vertices), begin (tris), end (tris));

    if ((selected >= shift) && (selected-shift < r.h/elementHeight - 2)) {
        GLfloat yu = r.y - (selected-shift+1) * elementHeight;
        GLfloat yd = yu - elementHeight;
        GLfloat c = 0.4;

        vector<GLfloat> selrect {
            r.x,     yu, bg[0]+c, bg[1]+c, bg[2]+c, bg[3],
            r.x,     yd, bg[0]+c, bg[1]+c, bg[2]+c, bg[3],
            r.x+r.w, yu, bg[0]+c, bg[1]+c, bg[2]+c, bg[3],
            r.x,     yd, bg[0]+c, bg[1]+c, bg[2]+c, bg[3],
            r.x+r.w, yd, bg[0]+c, bg[1]+c, bg[2]+c, bg[3],
            r.x+r.w, yu, bg[0]+c, bg[1]+c, bg[2]+c, bg[3]
        };

        vertices.insert (end (vertices), begin (selrect), end (selrect));
    }
}

int List::getSelected() const
{
    return selected;
}
