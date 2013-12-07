precision mediump float;

uniform sampler2D u_TexUnit;
varying vec2 v_TexCoords;
varying vec4 v_Color;

void main()
{
    gl_FragColor = texture2D(u_TexUnit, v_TexCoords) * v_Color;
}
