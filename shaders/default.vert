// default.vert
#version 460 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec4 in_color;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec4 color;

void main(void) {
  color = in_color;
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_position, 1.0);
}
