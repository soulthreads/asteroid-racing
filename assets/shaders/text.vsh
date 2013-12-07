uniform mat4 u_MvpMatrix;

uniform vec4 u_Colors[20];

attribute vec4 a_Position;
attribute vec2 a_TexCoords;
attribute float a_Color;

varying vec2 v_TexCoords;
varying vec4 v_Color;

void main()
{
    v_TexCoords = a_TexCoords;
    gl_Position = u_MvpMatrix * a_Position;
    v_Color = u_Colors[int(a_Color)];
}
