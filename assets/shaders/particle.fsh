precision mediump float;
uniform vec3 u_Color;
uniform sampler2D u_TexUnit;

varying float v_LifeTime;

void main(void)
{
    gl_FragColor = vec4 (u_Color / (1.0+v_LifeTime*v_LifeTime), 1.0) *
            texture2D (u_TexUnit, gl_PointCoord);
}
