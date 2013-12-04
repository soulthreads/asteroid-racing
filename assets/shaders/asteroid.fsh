precision mediump float;

uniform vec3 u_LightPos;
uniform vec3 u_Color;
uniform vec3 u_Color2;

varying vec3 v_Position;
varying vec3 v_FromCenter;
varying vec3 v_Normal;

void main ()
{
    vec3 lightVector = normalize (u_LightPos - v_Position);

    float ambient = 0.2;
    float diffuse = max (dot (v_Normal, lightVector), 0.0);

    vec3 v_Color = mix (u_Color2, u_Color, length(v_FromCenter)-0.3);

    gl_FragColor = vec4 (v_Color, 1.0) * (ambient + diffuse);
}
