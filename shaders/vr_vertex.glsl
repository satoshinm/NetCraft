#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

uniform vec4 matrix;

varying vec2 vUv;

void main() {
    vUv = uv;
    gl_Position = matrix * vec4( position, 1.0 );
}
