#include "layout.h"

Layout::Layout()
{
    elements.push_back (unique_ptr<Element> (
                            new Button ("Test", Rect (-0.5, 0.5, 0.5, 0.15), vec4 (0.5), vec4 (1))
                            ));
}

void Layout::draw()
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
