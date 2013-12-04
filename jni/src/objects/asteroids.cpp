#include "asteroids.h"

void Asteroids::createIcosphere(uint subdivisions)
{
    auto phi = (1.0 + sqrt (5.0)) / 2.0;

    icoVertices.clear ();
    icoIndexes.clear ();

    // 12 vertices of the icosahedron
    icoVertices.push_back (normalize (vec3 (-1,  phi, 0)));
    icoVertices.push_back (normalize (vec3 ( 1,  phi, 0)));
    icoVertices.push_back (normalize (vec3 (-1, -phi, 0)));
    icoVertices.push_back (normalize (vec3 ( 1, -phi, 0)));

    icoVertices.push_back (normalize (vec3 (0, -1,  phi)));
    icoVertices.push_back (normalize (vec3 (0,  1,  phi)));
    icoVertices.push_back (normalize (vec3 (0, -1, -phi)));
    icoVertices.push_back (normalize (vec3 (0,  1, -phi)));

    icoVertices.push_back (normalize (vec3 ( phi, 0, -1)));
    icoVertices.push_back (normalize (vec3 ( phi, 0,  1)));
    icoVertices.push_back (normalize (vec3 (-phi, 0, -1)));
    icoVertices.push_back (normalize (vec3 (-phi, 0,  1)));

    index = 12;

    // 20 triangles of the icosahedron
    // 5 faces around point 0
    icoIndexes.push_back (uvec3 (0, 11, 5));
    icoIndexes.push_back (uvec3 (0, 5, 1));
    icoIndexes.push_back (uvec3 (0, 1, 7));
    icoIndexes.push_back (uvec3 (0, 7, 10));
    icoIndexes.push_back (uvec3 (0, 10, 11));

    // 5 adjacent faces
    icoIndexes.push_back (uvec3 (1, 5, 9));
    icoIndexes.push_back (uvec3 (5, 11, 4));
    icoIndexes.push_back (uvec3 (11, 10, 2));
    icoIndexes.push_back (uvec3 (10, 7, 6));
    icoIndexes.push_back (uvec3 (7, 1, 8));

    // 5 faces around point 3
    icoIndexes.push_back (uvec3 (3, 9, 4));
    icoIndexes.push_back (uvec3 (3, 4, 2));
    icoIndexes.push_back (uvec3 (3, 2, 6));
    icoIndexes.push_back (uvec3 (3, 6, 8));
    icoIndexes.push_back (uvec3 (3, 8, 9));

    // 5 adjacent faces
    icoIndexes.push_back (uvec3 (4, 9, 5));
    icoIndexes.push_back (uvec3 (2, 4, 11));
    icoIndexes.push_back (uvec3 (6, 2, 10));
    icoIndexes.push_back (uvec3 (8, 6, 7));
    icoIndexes.push_back (uvec3 (9, 8, 1));

    for (int i = 0; i < subdivisions; ++i) {
        vector<uvec3> indexes2;
        for (auto &tri : icoIndexes) {
            // replace triangle by 4 triangles
            auto a = getMiddlePoint (tri[0], tri[1]);
            auto b = getMiddlePoint (tri[1], tri[2]);
            auto c = getMiddlePoint (tri[2], tri[0]);

            indexes2.push_back (uvec3 (tri[0], a, c));
            indexes2.push_back (uvec3 (tri[1], b, a));
            indexes2.push_back (uvec3 (tri[2], c, b));
            indexes2.push_back (uvec3 (a, b, c));
        }
        icoIndexes = indexes2;
    }
}

GLushort Asteroids::getMiddlePoint(GLushort p1, GLushort p2)
{
    GLushort smallerIndex = p1 < p2 ? p1 : p2;
    GLushort greaterIndex = p1 < p2 ? p2 : p1;
    uint key = (smallerIndex << 16) + greaterIndex;

    auto iter = icoIndexCache.find (key);
    if (iter != icoIndexCache.end ())
        return iter->second;

    vec3 middle = normalize (icoVertices[p1]+icoVertices[p2]);
    icoVertices.push_back (middle);
    icoIndexCache[key] = index;
    return index++;
}

Asteroids::Asteroids()
{
    program = buildProgramFromAssets ("shaders/asteroid.vsh", "shaders/asteroid.fsh");
    validateProgram (program);
    u_MvpMatrixHandle = glGetUniformLocation (program, "u_MvpMatrix");
    u_MvMatrixHandle = glGetUniformLocation (program, "u_MvMatrix");
    u_LightPosHandle = glGetUniformLocation (program, "u_LightPos");

    a_PositionHandle = glGetAttribLocation (program, "a_Position");
    a_NormalHandle = glGetAttribLocation (program, "a_Normal");

    createIcosphere (3);

    for (auto &v : icoVertices) {
        v += (0.2f*simplex(v)+0.1f*simplex(v*2.f) + 0.05f*simplex (v*4.f)) * v;
    }

    vector<GLfloat> vertexData;
    for (auto &v : icoVertices) {
        vertexData.push_back (v[0]);
        vertexData.push_back (v[1]);
        vertexData.push_back (v[2]);
    }

    vector<GLushort> indexData;
    for (auto &i : icoIndexes) {
        indexData.push_back (i[0]);
        indexData.push_back (i[1]);
        indexData.push_back (i[2]);
    }

    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, vertexData.size () * sizeof (GLfloat), vertexData.data (), GL_STATIC_DRAW);

    glGenBuffers (1, &ibo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, indexData.size () * sizeof (GLfloat), indexData.data (), GL_STATIC_DRAW);

    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

    // TODO: add bump mapping
}

Asteroids::~Asteroids ()
{
    if (vbo) {
        glDeleteBuffers (1, &vbo);
        vbo = 0;
    }

    if (ibo) {
        glDeleteBuffers (1, &ibo);
        ibo = 0;
    }
}

void Asteroids::draw(Engine &engine)
{
    glUseProgram (program);

    mat4 mv = engine.viewMatrix * translate (scale(mat4 (1), vec3(2)), vec3 (0, 0, 5));
    mat4 mvp = engine.projectionMatrix * mv;
    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, value_ptr (mvp));
    glUniformMatrix4fv (u_MvMatrixHandle, 1, GL_FALSE, value_ptr (mv));
    glUniform3fv (u_LightPosHandle, 1, value_ptr (engine.viewMatrix * engine.state.lightPos));

    glBindBuffer (GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer (a_PositionHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray (a_PositionHandle);

    glVertexAttribPointer (a_NormalHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray (a_NormalHandle);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
    int size;
    glGetBufferParameteriv (GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements (GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
}
