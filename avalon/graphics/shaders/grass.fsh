"                                                                                                   \n\
#ifdef GL_ES                                                                                        \n\
precision lowp float;                                                                               \n\
#endif                                                                                              \n\
                                                                                                    \n\
varying vec2 v_texCoord;                                                                            \n\
uniform sampler2D u_texture;                                                                        \n\
uniform float u_time;                                                                               \n\
                                                                                                    \n\
const float speed = 2.0;                                                                            \n\
const float bendFactor = 0.2;                                                                       \n\
void main()                                                                                         \n\
{                                                                                                   \n\
  float height = 1.0 - v_texCoord.y;                                                                \n\
  float offset = pow(height, 2.5);                                                                  \n\
                                                                                                    \n\
  offset *= (sin(u_time * speed) * bendFactor);                                                     \n\
                                                                                                    \n\
  vec3 normalColor = texture2D(u_texture, fract(vec2(v_texCoord.x + offset, v_texCoord.y))).rgb;    \n\
  gl_FragColor = vec4(normalColor, 1);                                                              \n\
}                                                                                                   \n\
                                                                                                    \n\
";
