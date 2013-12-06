#include "text.h"

Text::Text()
{
    stride = (2 + 2) * sizeof (GLfloat);

    program = buildProgramFromAssets ("shaders/text.vsh", "shaders/text.fsh");
    validateProgram (program);
    u_MvpMatrixHandle = glGetUniformLocation (program, "u_MvpMatrix");
    u_TexUnitHandle = glGetUniformLocation (program, "u_TexUnit");

    a_PositionHandle = glGetAttribLocation (program, "a_Position");
    a_TexCoordsHandle = glGetAttribLocation (program, "a_TexCoords");
//    a_ColorHandle = glGetAttribLocation (program, "a_Color");

    fontTex = loadTextureFromAsset ("textures/terminus.png");
    glActiveTexture (GL_TEXTURE4);
    glBindTexture (GL_TEXTURE_2D, fontTex);
}

Text::~Text()
{
    if (fontTex) {
        glDeleteTextures (1, &fontTex);
        fontTex = 0;
    }
}

vector<GLfloat> Text::makeSymbol(vec2 pos, float size, char c)
{
    int offset = c-32;
    if (offset < 0) offset = 0;
    float x = (offset % roww) * w;
    float y = (offset / roww) * h;

    vector<GLfloat> ret = {pos[0], pos[1]+size*h, x, y,
                          pos[0], pos[1], x, y+h,
                          pos[0]+size*w, pos[1]+size*h, x+w, y,
                          pos[0], pos[1], x, y+h,
                          pos[0]+size*w, pos[1], x+w, y+h,
                          pos[0]+size*w, pos[1]+size*h, x+w, y};
    return ret;
}

void Text::draw(const Engine &engine)
{
    if (changed) {
        updateVertexData ();
        changed = false;
    }
    if (vertexData.size ()) {
        glUseProgram (program);
        glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, value_ptr (engine.orthoMatrix));

        glUniform1i (u_TexUnitHandle, 4);

        glVertexAttribPointer (a_PositionHandle, 2, GL_FLOAT, GL_FALSE, stride, &vertexData[0]);
        glEnableVertexAttribArray (a_PositionHandle);

        glVertexAttribPointer (a_TexCoordsHandle, 2, GL_FLOAT, GL_FALSE, stride, &vertexData[2]);
        glEnableVertexAttribArray (a_TexCoordsHandle);

        glDisable (GL_DEPTH_TEST);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays (GL_TRIANGLES, 0, vertexData.size () / 4);
        glDisable (GL_BLEND);
        glEnable (GL_DEPTH_TEST);
    }
}

void Text::updateVertexData ()
{
    vertexData.erase (vertexData.begin (), vertexData.end ());
    for (auto &kv : strings) {
        auto t = kv.second;
        auto pos = t.position;
        pos[0] -= (t.ax/2.0) * t.text.size() * t.size * w;
        pos[1] -= (t.ay/2.0) * t.size * h;
        for (char c : t.text) {
            auto symbol = makeSymbol (pos, t.size, c);
            vertexData.insert (vertexData.end (), symbol.begin (), symbol.end ());
            pos[0] += t.size * w;
        }
    }
}

void Text::addText(const string &key, const textUnit &text)
{
    strings[key] = text;
    changed = true;
}

void Text::deleteText (const string &key)
{
    strings.erase (key);
    changed = true;
}
