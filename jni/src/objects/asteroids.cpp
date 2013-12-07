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
    createIcosphere (3);

    for (auto &i : icoIndexes) {
        indexData.push_back (i[0]);
        indexData.push_back (i[1]);
        indexData.push_back (i[2]);
    }

    // TODO: add bump mapping

    explosion = unique_ptr<Particles> (new Particles (vec3 (1, 0.5, 0.1), particlesCount,
                                                      engine.width/2, 1/1024.0));
}

Asteroids::~Asteroids ()
{
    for (auto &a : asteroids)
        if (a.vbo) {
            glDeleteBuffers (1, &a.vbo);
            a.vbo = 0;
        }

    if (ibo) {
        glDeleteBuffers (1, &ibo);
        ibo = 0;
    }
}

void Asteroids::addAsteroid (vec3 position, float radius) {
    vector<vec3> tmpVertices = icoVertices;
    int i = asteroids.size ();
    for (auto &v : tmpVertices) {
        v += (0.25f*simplex((vec3(i)+v))
              + 0.125f*simplex((vec3(i)+v)*2.f)
              + 0.0625f*simplex ((vec3(i)+v)*4.f)
              + 0.03125f*simplex ((vec3(i)+v)*8.f)) * v;
    }

    vector<GLfloat> vertices;
    for (auto &v : tmpVertices) {
        vertices.push_back (v[0]);
        vertices.push_back (v[1]);
        vertices.push_back (v[2]);
    }
    asteroids.push_back (asteroid {createVBO (vertices), vertices,
                                   position,
                                   ballRand (2.f),
                                   angleAxis(linearRand (-1.f, 1.f), ballRand (1.f)),
                                   angleAxis(linearRand (.5f, .5f), ballRand (1.f)),
                                   radius, 1.0, false});
}

void Asteroids::init () {
    token = engine.token;

    program = buildProgramFromAssets ("shaders/asteroid.vsh", "shaders/asteroid.fsh");
    validateProgram (program);
    u_MvpMatrixHandle = glGetUniformLocation (program, "u_MvpMatrix");
    u_MvMatrixHandle = glGetUniformLocation (program, "u_MvMatrix");
    u_LightPosHandle = glGetUniformLocation (program, "u_LightPos");
    u_ColorHandle = glGetUniformLocation (program, "u_Color");
    u_Color2Handle = glGetUniformLocation (program, "u_Color2");

    a_PositionHandle = glGetAttribLocation (program, "a_Position");
    a_NormalHandle = glGetAttribLocation (program, "a_Normal");

    glGenBuffers (1, &ibo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, indexData.size () * sizeof (GLushort), indexData.data (), GL_STATIC_DRAW);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

    for (auto &a : asteroids) {
        if (a.vbo) {
            glDeleteBuffers (1, &a.vbo);
        }
        a.vbo = createVBO (a.vertices);
    }
}

GLuint Asteroids::createVBO (vector<GLfloat> &vertices) {
    GLuint vbo;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, vertices.size () * sizeof (GLfloat), vertices.data (), GL_STATIC_DRAW);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    return vbo;
}

void Asteroids::draw()
{
    if (token != engine.token) init ();

    glUseProgram (program);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
    int size;
    glGetBufferParameteriv (GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    for (auto it = asteroids.begin (); it != asteroids.end ();) {
        if (it->stamina <= 0) it->blownUp = true;
        if (it->blownUp) {
            for (int i = 0; i < particlesCount/2; i++)
                explosion->addParticles (it->position+sphericalRand(it->radius),
                                         sphericalRand (60.0f) + ballRand (40.0f), 1);
            glDeleteBuffers (1, &it->vbo);
            it = asteroids.erase (it);
        } else {
            ++it;
        }
    }

    for (auto &a : asteroids) {
        if (a.blownUp) continue;
        a.position += a.velocity * float(engine.delta * 0.001);
        a.orientation = normalize (a.orientation * a.rot);
        mat4 mv = engine.viewMatrix
                * translate (mat4(1), a.position)
                * scale(mat4 (1), vec3(a.radius))
                * mat4_cast (a.orientation);
        mat4 mvp = engine.projectionMatrix * mv;
        glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, value_ptr (mvp));
        glUniformMatrix4fv (u_MvMatrixHandle, 1, GL_FALSE, value_ptr (mv));
        glUniform3fv (u_LightPosHandle, 1, value_ptr (engine.viewMatrix * engine.state.lightPos));

        glUniform3fv (u_ColorHandle, 1,
                      value_ptr (mix(vec3(1.0, 0.1, 0.1), vec3 (0.9, 0.7, 0.5), a.stamina)));
        glUniform3fv (u_Color2Handle, 1,
                      value_ptr (mix(vec3(1.0, 0.5, 0.3), vec3 (0.1, 0.05, 0.05), a.stamina)));


        glBindBuffer (GL_ARRAY_BUFFER, a.vbo);

        glVertexAttribPointer (a_PositionHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray (a_PositionHandle);

        glVertexAttribPointer (a_NormalHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray (a_NormalHandle);

        glDrawElements (GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    }

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    explosion->draw ();
}

vector<asteroid> &Asteroids::getAsteroids()
{
    return asteroids;
}

void Asteroids::reset () {
    for (auto &a : asteroids)
        if (a.vbo)
            glDeleteBuffers (1, &a.vbo);

    asteroids.clear ();

    explosion->reset ();
}
