// default.frag
#version 460 core
out vec4 FragColor;
 
in vec4 color;

void main(void) {
	FragColor = color;
}