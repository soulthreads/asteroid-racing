precision mediump float;

uniform vec3 u_LightPos;

varying vec3 v_Position;
varying vec3 v_FromCenter;
varying vec3 v_Normal;

void main ()
{
    vec3 lightVector = normalize (u_LightPos - v_Position);

    float ambient = 0.2;
    float diffuse = max (dot (v_Normal, lightVector), 0.0);

    vec3 v_Color = mix (vec3 (0.4, 0.3, 0.2), vec3 (0.9, 0.7, 0.5), length(v_FromCenter)-0.5);

    gl_FragColor = vec4 (v_Color, 1.0) * (ambient + diffuse);
}
