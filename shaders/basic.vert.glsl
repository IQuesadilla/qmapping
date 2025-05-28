#version 100
#ifdef GL_ES
precision mediump float;
#endif

attribute vec3 aPos;
uniform mat4 mvp;
varying float depth;

void main() {
  vec4 pos4 = mvp * vec4(aPos, 1.0);
  depth = pos4.z;
  gl_Position = pos4;
}
