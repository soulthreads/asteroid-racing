#ifndef SHADER_H
#define SHADER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

GLuint compileShader (const GLenum type, const GLchar *source, const GLint length);
GLuint linkProgram (const GLuint vertexShader, const GLuint fragmentShader);
GLuint buildProgram (const GLchar *vertexShaderSource, const GLint vertexShaderLength,
                     const GLchar *fragmentShaderSource, const GLint fragmentShaderLength);
GLint validateProgram (const GLuint program);

#endif // SHADER_H
