uniform mat4 u_MvpMatrix;
uniform mat4 u_MvMatrix;

attribute vec4 a_Position;
attribute vec3 a_Normal;
attribute vec4 a_Color;

varying vec3 v_Position;
varying vec4 v_Color;
varying vec3 v_Normal;

void main () {
    v_Position = vec3 (u_MvMatrix * a_Position);
    v_Color = a_Color;
    v_Normal = normalize(vec3 (u_MvMatrix * vec4(a_Normal, 0.0)));

    gl_Position = u_MvpMatrix * a_Position;
}
