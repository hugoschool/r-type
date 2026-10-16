uniform sampler2D texture; // SFML automatically binds the current frame/texture here
uniform int mode;          // 0 = Normal, 1 = Protanopia (Red), 2 = Deuteranopia (Green), 3 = Tritanopia (Blue)

void main() {
    // Fetch current pixel color
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec3 rgb = pixel.rgb;

    // Convert to LMS color space
    float L = (17.8824 * rgb.r) + (43.5161 * rgb.g) + (4.1193 * rgb.b);
    float M = (3.4557 * rgb.r) + (27.1554 * rgb.g) + (3.8671 * rgb.b);
    float S = (0.02996 * rgb.r) + (0.1843 * rgb.g) + (1.4670 * rgb.b);

    float l = L, m = M, s = S;

    // Simulate deficiencies
    if (mode == 1) { // Protanopia
        l = 0.0 * L + 2.02344 * M - 2.52581 * S;
    } else if (mode == 2) { // Deuteranopia
        m = 0.494207 * L + 0.0 * M + 1.24827 * S;
    } else if (mode == 3) { // Tritanopia
        s = -0.395913 * L + 0.801109 * M + 0.0 * S;
    }

    // Convert back to RGB space
    vec3 sim;
    sim.r = (0.080944 * l) + (-0.130504 * m) + (0.116721 * s);
    sim.g = (-0.0102485 * l) + (0.0540194 * m) + (-0.113615 * s);
    sim.b = (-0.000365296 * l) + (-0.00412161 * m) + (0.693511 * s);

    // Isolate lost color data & perform Daltonization shift
    vec3 error = rgb - sim;
    vec3 correction;
    correction.r = (error.r * 0.0) + (error.g * 0.0) + (error.b * 0.0);
    correction.g = (error.r * 0.7) + (error.g * 1.0) + (error.b * 0.0);
    correction.b = (error.r * 0.7) + (error.g * 0.0) + (error.b * 1.0);

    // Add corrected detail to original image if mode is active
    if (mode > 0) {
        gl_FragColor = vec4(rgb + correction, pixel.a);
    } else {
        gl_FragColor = pixel;
    }
}