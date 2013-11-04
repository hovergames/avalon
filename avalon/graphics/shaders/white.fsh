"                                                                                    \n\
#ifdef GL_ES                                                                         \n\
precision lowp float;                                                                \n\
#endif                                                                               \n\
                                                                                     \n\
varying vec2 v_texCoord;                                                             \n\
varying vec4 v_color;                                                                \n\
                                                                                     \n\
uniform sampler2D u_texture;                                                         \n\
                                                                                     \n\
void main(void) {                                                                    \n\
    vec4 texColor = texture2D(u_texture, v_texCoord);                                \n\
    float grey = (0.21 * texColor.r + 0.71 * texColor.g + 0.07 * texColor.b) * 2.0;  \n\
    gl_FragColor = v_color * vec4(grey, grey, grey, texColor.a);                     \n\
}                                                                                    \n\
";
