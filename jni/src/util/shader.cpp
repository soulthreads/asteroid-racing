#include "shader.h"

#include <cstdlib>
#include <cstring>
#include <cassert>

#define LOG_TAG "AR_shaders"
#include "logs.h"

#define DEBUG_LOG 1

GLuint compileShader (const GLenum type, const GLchar *source, const GLint length) {
    assert (source != nullptr);
    GLuint shaderObjectId = glCreateShader (type);
    GLint compileStatus;

    assert (shaderObjectId != nullptr);

    glShaderSource (shaderObjectId, 1, &source, &length);
    glCompileShader (shaderObjectId);
    glGetShaderiv (shaderObjectId, GL_COMPILE_STATUS, &compileStatus);

#if defined(DEBUG_LOG)
    GLint logLength;
    glGetShaderiv (shaderObjectId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *) malloc (logLength);
        glGetShaderInfoLog (shaderObjectId, logLength, &logLength, log);
        LOGI ("Results of compiling shader source:\n%s", log);
        free (log);
    }
#endif

    assert (compileStatus != 0);

    return shaderObjectId;
}

GLuint linkProgram (const GLuint vertexShader, const GLuint fragmentShader) {
    GLuint programObjectId = glCreateProgram ();
    GLint linkStatus;

    assert (programObjectId != 0);

    glAttachShader (programObjectId, vertexShader);
    glAttachShader (programObjectId, fragmentShader);
    glLinkProgram (programObjectId);
    glGetProgramiv (programObjectId, GL_LINK_STATUS, &linkStatus);

#if defined(DEBUG_LOG)
    GLint logLength;
    glGetProgramiv (programObjectId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *) malloc (logLength);
        glGetProgramInfoLog (programObjectId, logLength, &logLength, log);
        LOGI ("Results of linking program:\n%s", log);
        free (log);
    }
#endif

    assert (linkStatus != 0);

    return programObjectId;
}

GLuint buildProgram (const GLchar *vertexShaderSource, const GLint vertexShaderLength,
                     const GLchar *fragmentShaderSource, const GLint fragmentShaderLength) {
    assert (vertexShaderSource != nullptr);
    assert (fragmentShaderSource != nullptr);

    GLuint vertexShader = compileShader (GL_VERTEX_SHADER, vertexShaderSource, vertexShaderLength);
    GLuint fragmentShader = compileShader (GL_FRAGMENT_SHADER, fragmentShaderSource, fragmentShaderLength);

    return linkProgram (vertexShader, fragmentShader);
}

GLint validateProgram (const GLuint program) {
#if defined (DEBUG_LOG)
    GLint validateStatus, logLength;

    glValidateProgram (program);
    glGetProgramiv (program, GL_VALIDATE_STATUS, &validateStatus);
    glGetProgramiv (program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *) malloc (logLength);
        glGetProgramInfoLog (program, logLength, &logLength, log);
        LOGI ("Results of validating program:\n%s", log);
        free (log);
    }

    return validateStatus;
#else
    return 0;
#endif
}
