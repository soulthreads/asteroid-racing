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

vector<GLfloat> Text::makeSymbol(vec2 pos, float size, uint ch)
{
    // Begin UTF-8 madness.
    int offset = 0;
    if (ch == 0xD081) offset = 95;
    else if (ch == 0xD191) offset = 120;
    else if (ch >= 0xD090 && ch <= 0xD0BF) offset = (ch & 0xFF) - 15-33;
    else if (ch >= 0xD180 && ch <= 0xD18F) offset = (ch & 0xFF) + 16;
    else if (ch >= 0x0020 && ch <= 0x007E) offset = ch - 0x20;
    // End UTF-8 madness.

    float sw = symw / texw, th = symh / texh;
    float s = (offset % rowc) * sw;
    float t = (offset / rowc) * th;

    float pw = factor * symw, ph = factor * symh;
    vector<GLfloat> ret = {
        pos[0],         pos[1]+size*ph, s,    t,
        pos[0],         pos[1],         s,    t+th,
        pos[0]+size*pw, pos[1]+size*ph, s+sw, t,
        pos[0],         pos[1],         s,    t+th,
        pos[0]+size*pw, pos[1],         s+sw, t+th,
        pos[0]+size*pw, pos[1]+size*ph, s+sw, t
    };
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
        uint width = 0;
        for (char c : t.text) { if (c < 0xC0) ++width; }
        pos[0] -= (t.ax/2.0) * width * t.size * factor * symw;
        pos[1] -= (t.ay/2.0) * t.size * factor * symh;
        int i = 0;
        while (i < t.text.size ()) {
            unsigned char c1 = t.text[i], c2 = t.text[i+1];
            uint ch = 0;
            if (c1 < 0xC0) { // ASCII character
                ch = c1;
                i += 1;
            } else { // UTF-8 character
                ch = (c1 << 8) | c2;
                i += 2;
            }
            auto symbol = makeSymbol (pos, t.size, ch);
            vertexData.insert (vertexData.end (), symbol.begin (), symbol.end ());
            pos[0] += t.size * factor * symw;
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
