#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

attribute vec3 position;

varying vec2 vUv;

void main() {
    vUv = (position.xy+vec2(1,1))/2.0;
    gl_Position = vec4( position, 1.0 );
}
