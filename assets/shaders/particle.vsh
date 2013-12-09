uniform mat4 u_MvpMatrix;
uniform float u_Size;

attribute vec4 a_Position;
attribute float a_LifeTime;

varying float v_LifeTime;

void main(void)
{
    gl_Position = u_MvpMatrix * a_Position;
    v_LifeTime = a_LifeTime;
    gl_PointSize = u_Size * 2.0 / (1.0+sqrt(abs(gl_Position.w)));
}
