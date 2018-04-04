// particle.vert
#version 460 core
layout (location = 0) in vec4 in_position;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec4 u_color;

out vec4 color;

void main(void) {
  color = u_color;
  gl_Position = projectionMatrix * viewMatrix * vec4(in_position.xyz, 1.0);
}
