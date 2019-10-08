#version 330 core

layout(location = 0) in vec2 pos;
out vec2 uv;

uniform mat4 worldTransform;
uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;

void main() {
    mat4 MVM = inverse(cameraTransform) * worldTransform;
    vec4 fragPos = MVM * vec4(pos, 0, 1);
    gl_Position = projectionMatrix * fragPos;
    uv = (pos + vec2(1)) / 2;
}