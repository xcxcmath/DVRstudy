#version 330 core
in vec4 fragPos;
in vec3 objectPos;
out vec3 outputColor;

void main() {
    outputColor = (objectPos + vec3(1)) / 2;
}