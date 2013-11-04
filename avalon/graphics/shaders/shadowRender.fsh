"                                                                   \n\
#ifdef GL_ES                                                        \n\
precision lowp float;                                               \n\
#endif                                                              \n\
                                                                    \n\
#define PI 3.14                                                     \n\
varying vec2 v_texCoord;                                            \n\
varying vec4 v_color;                                               \n\
                                                                    \n\
uniform sampler2D u_texture;                                        \n\
uniform sampler2D u_texture2;                                       \n\
uniform vec2 resolution;                                            \n\
                                                                    \n\
uniform float softShadows;                                          \n\
                                                                    \n\
//sample from the distance map                                      \n\
float sample(vec2 coord, float r) {                                 \n\
    return step(r, texture2D(u_texture2, coord).r);                 \n\
}                                                                   \n\
                                                                    \n\
void main(void) {                                                   \n\
    //rectangular to polar                                          \n\
    vec2 norm = v_texCoord.st * 2.0 - 1.0;                          \n\
    float theta = atan(norm.y, norm.x);                             \n\
                                                                    \n\
    /* example: directional light! */                               \n\
    /*if (theta < -0.7 || theta > 0.3) {                            \n\
        gl_FragColor = vec4(0,0,0,0);                               \n\
        return;                                                     \n\
    }                                                               \n\
     */                                                             \n\
                                                                    \n\
    float r = length(norm);                                         \n\
    float coord = (theta + PI) / (2.0*PI);                          \n\
                                                                    \n\
    //the tex coord to sample our 1D lookup texture2D               \n\
    //always 0.0 on y axis                                          \n\
    vec2 tc = vec2(coord, 0.0);                                     \n\
                                                                    \n\
    //the center tex coord, which gives us hard shadows             \n\
    float center = sample(vec2(tc.x, tc.y), r);                     \n\
                                                                    \n\
    //we multiply the blur amount by our distance from center       \n\
    //this leads to more blurriness as the shadow fades away        \n\
    float blur = (1./resolution.x)  * smoothstep(0., 1., r);        \n\
                                                                    \n\
    //now we use a simple gaussian blurriness                       \n\
    float sum = 0.0;                                                \n\
                                                                    \n\
    sum += sample(vec2(tc.x - 4.0*blur, tc.y), r) * 0.05;           \n\
    sum += sample(vec2(tc.x - 3.0*blur, tc.y), r) * 0.09;           \n\
    sum += sample(vec2(tc.x - 2.0*blur, tc.y), r) * 0.12;           \n\
    sum += sample(vec2(tc.x - 1.0*blur, tc.y), r) * 0.15;           \n\
                                                                    \n\
    sum += center * 0.16;                                           \n\
                                                                    \n\
    sum += sample(vec2(tc.x + 1.0*blur, tc.y), r) * 0.15;           \n\
    sum += sample(vec2(tc.x + 2.0*blur, tc.y), r) * 0.12;           \n\
    sum += sample(vec2(tc.x + 3.0*blur, tc.y), r) * 0.09;           \n\
    sum += sample(vec2(tc.x + 4.0*blur, tc.y), r) * 0.05;           \n\
                                                                    \n\
    //1.0 -> in light, 0.0 -> in shadow                             \n\
    float lit = mix(center, sum, softShadows);                      \n\
    //multiply the summed amount by our distance, which             \n\
    //gives us a radial falloff                                     \n\
    //then multiply by vertex (light) color                         \n\
    gl_FragColor = v_color * vec4(lit * smoothstep(1.0, 0.0, r));   \n\
}                                                                   \n\
";



