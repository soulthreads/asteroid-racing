uniform mat4 u_MvpMatrix;
uniform float u_Size;

attribute vec4 a_Position;

void main(void)
{
    gl_Position = u_MvpMatrix * a_Position;
    gl_PointSize = u_Size;
}
