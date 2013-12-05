precision mediump float;

uniform sampler2D u_TexUnit;
varying vec2 v_TexCoords;

void main()
{
    gl_FragColor = texture2D(u_TexUnit, v_TexCoords);
}
