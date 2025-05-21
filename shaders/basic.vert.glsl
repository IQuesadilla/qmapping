#version 100
#ifdef GL_ES
precision mediump float;
#endif

attribute vec3 aPos;
uniform mat4 mvp;

void main() {
  gl_Position = mvp * vec4(aPos, 1.0);
}
