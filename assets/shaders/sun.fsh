precision mediump float;
uniform sampler2D u_TexUnit;

void main(void)
{
    gl_FragColor = vec4(1.0) * texture2D (u_TexUnit, gl_PointCoord);
}
