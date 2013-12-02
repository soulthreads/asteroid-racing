precision mediump float;

uniform vec3 u_LightPos;
uniform vec3 u_EyePos;

varying vec3 v_Position;
varying vec4 v_Color;
varying vec3 v_Normal;

void main()
{
    float distance = length (u_LightPos - v_Position);
    vec3 lightVector = normalize (u_LightPos - v_Position);
    vec3 eyeVector = normalize (u_EyePos - v_Position);

    float ambient = 0.2;
    float diffuse = max (dot (v_Normal, lightVector), 0.0);
//    diffuse *= 1.0 / (1.0 + (0.01 * distance * distance));

    float specular = 0.0;
    if (diffuse > 0.0) {
        specular = pow (max (dot (eyeVector, normalize (reflect (-lightVector, v_Normal))), 0.0), 100.0);
    }

    gl_FragColor = v_Color * (ambient + diffuse) + specular;
}
