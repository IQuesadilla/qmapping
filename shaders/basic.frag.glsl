#version 100
#ifdef GL_ES
precision mediump float;
#endif

varying float depth;

void main () {
  gl_FragColor = vec4(depth, 0.0, 0.0, 1.0);
}
