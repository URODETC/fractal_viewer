#version 150

uniform vec2 u_resolution;
uniform vec2 u_center;      // world center (double precision on CPU, but passed as float vec2)
uniform vec2 u_scale;       // world width and height
uniform int u_maxIterations;
uniform float u_escapeRadius; // default 4.0

out vec4 FragColor;

// continuous coloring palette (cos palette)
vec3 palette(float t) {
    float r = 0.5 + 0.5 * cos(6.28318 * (t + 0.00));
    float g = 0.5 + 0.5 * cos(6.28318 * (t + 0.33));
    float b = 0.5 + 0.5 * cos(6.28318 * (t + 0.67));
    return vec3(r, g, b);
}

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution;

    // map to complex plane (use float precision; for deeper zooms consider perturbation method)
    float x0 = u_center.x + (uv.x - 0.5) * u_scale.x;
    float y0 = u_center.y + (uv.y - 0.5) * u_scale.y;

    float x = 0.0;
    float y = 0.0;

    int iter = 0;
    float x2 = 0.0;
    float y2 = 0.0;

    // optimized loop: local variables, early bailout
    for (int i = 0; i < u_maxIterations; ++i) {
        // x^2 - y^2 + x0
        y = 2.0 * x * y + y0;
        x = x2 - y2 + x0;

        x2 = x * x;
        y2 = y * y;

        if (x2 + y2 > u_escapeRadius) {
            iter = i;
            // continuous iteration count using log (smooth coloring)
            float log_zn = log(x2 + y2) * 0.5;
            float nu = log(log_zn / log(2.0)) / log(2.0);
            float it = float(i) + 1.0 - nu;
            float t = it / float(u_maxIterations);
            vec3 col = palette(t);
            FragColor = vec4(col, 1.0);
            return;
        }
    }

    // inside set -> black (or deep color)
    vec3 inside = vec3(0.0, 0.0, 0.0);
    FragColor = vec4(inside, 1.0);
}
