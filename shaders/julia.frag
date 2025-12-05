#version 150

uniform vec2 u_resolution;
uniform vec2 u_center;
uniform vec2 u_scale;
uniform int u_maxIterations;
uniform float u_escapeRadius;

out vec4 FragColor;

vec3 hsv2rgb(vec3 c){
    vec3 p = abs(mod(c.x*6.0 + vec3(0.0,4.0,2.0), 6.0) - 3.0);
    p = clamp(p - 1.0, 0.0, 1.0);
    return c.z * mix(vec3(1.0), p, c.y);
}

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution;
    vec2 c = vec2(-0.8, .156);

    int iter = 0;
    float mu;
    vec2 z = vec2((uv.x- 0.5) * u_scale.x, (uv.y - 0.5) * u_scale.y) + u_center;
    for (int i; i < u_maxIterations; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (2.0 * z.x * z.y) + c.y;
        z = vec2(x,y);
        if(dot(z,z) > u_escapeRadius) {
            float log_zn = log(dot(z,z)) * 0.5;
            float nu = log(log_zn / log(2.0)) / log(2.0);
            mu = float(i) + 1.0 - nu;
            break;
        }
    }

    if(iter == 0 && mu == 0.0) {
        FragColor = vec4(0,0,0,1);
        return;
    }
    float t = mu / float(u_maxIterations);
    t = pow(t,0.6);

    float hue = 0.65 + 1.2 * t;
    float sat = 0.7 + 0.3 * (1.0 - t);
    float val = 0.12 + 1.2 * t * (1.0 - 0.2*t);
    vec3 col = hsv2rgb(vec3(mod(hue,1.0), sat, val));

    FragColor = vec4(col,1);
    return;
}