#version 100
#ifdef GL_ES
precision mediump float;
#endif

varying vec2 vPos;

void main () {
  gl_FragColor = vec4(depth, 0.0, 0.0, 1.0);
}
