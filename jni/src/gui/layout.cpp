#include "layout.h"

#define LOG_TAG "AR_layout"
#include "util/logs.h"

Layout::Layout()
{
}

void Layout::draw ()
{
    if (token != engine.token) init ();
    glUseProgram (program);
    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, value_ptr (engine.orthoMatrix));

    vertices.clear ();
    for (auto &e : elements) {
        auto v = e->getVertices ();
        vertices.insert (vertices.end (), v.begin(), v.end ());
        for (auto &t : e->getTexts ()) {
            text->addText (e->getId (), t);
        }
    }

    text->addText ("name", name);

    glVertexAttribPointer (a_PositionHandle, 2, GL_FLOAT, GL_FALSE, Element::stride, &vertices[0]);
    glEnableVertexAttribArray (a_PositionHandle);

    glVertexAttribPointer (a_ColorHandle, 4, GL_FLOAT, GL_FALSE, Element::stride, &vertices[2]);

    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays (GL_TRIANGLES, 0, vertices.size () / Element::components);
    glDisable (GL_BLEND);
    glEnable (GL_DEPTH_TEST);
}

void Layout::init () {
    token = engine.token;

    program = buildProgramFromAssets ("shaders/gui.vsh", "shaders/gui.fsh");
    validateProgram (program);
    u_MvpMatrixHandle = glGetUniformLocation (program, "u_MvpMatrix");

    a_PositionHandle = glGetAttribLocation (program, "a_Position");
    a_ColorHandle = glGetAttribLocation (program, "a_Color");
}

void Layout::touchDown (float x, float y) {
    touchIndex = 0;
    for (auto &e : elements) {
        auto r = e->getRect ();
        if ((x >= r.x) && (x <= r.x+r.w) && (y >= r.y-r.h) && (y <= r.y)) {
            return;
        }
        touchIndex++;
    }
    touchIndex = -1;
}

void Layout::touchUp (float x, float y) {
    if (touchIndex != -1) {
        auto r = elements[touchIndex]->getRect ();
        if ((x >= r.x) && (x <= r.x+r.w) && (y >= r.y-r.h) && (y <= r.y)) {
            elements[touchIndex]->run ();
        }
    }
}

void Layout::addButton (const string &label, Rect rect, vec4 bgColor, vec4 fgColor, Functor f) {
    elements.push_back (unique_ptr<Element> (new Button (label, rect, bgColor, fgColor, f)));
}

void Layout::setName(const string &layoutName)
{
    name = textUnit {vec2 (0, 1), vec4 (1), 2, A_CENTER, A_PLUS, layoutName};
}
