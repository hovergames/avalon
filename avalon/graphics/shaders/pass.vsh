"                                             \n\
#ifdef GL_ES                                  \n\
precision lowp float;                         \n\
#endif                                        \n\
                                              \n\
attribute vec4 a_position;                    \n\
attribute vec2 a_texCoord;                    \n\
attribute vec4 a_color;                       \n\
                                              \n\
varying  vec2 v_texCoord;                     \n\
                                              \n\
varying vec4 v_color;                         \n\
                                              \n\
void main() {                                 \n\
    v_color = a_color;                        \n\
    gl_Position = CC_MVPMatrix * a_position;  \n\
    v_texCoord = a_texCoord;                  \n\
}                                             \n\
";
