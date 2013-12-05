uniform mat4 u_MvpMatrix;

attribute vec4 a_Position;
attribute vec2 a_TexCoords;

varying vec2 v_TexCoords;

void main()
{
    v_TexCoords = a_TexCoords;
    gl_Position = u_MvpMatrix * a_Position;
}
