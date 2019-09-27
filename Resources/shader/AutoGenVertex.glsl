#version 330 core

layout(location = 0) in vec3 pos;
out vec4 fragPos;
out vec4 projected;
out vec3 objectPos;

uniform mat4 worldTransform;
uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;

void main() {
    mat4 MVM = inverse(cameraTransform) * worldTransform;
    fragPos = MVM * vec4(pos, 1);
    projected = gl_Position = projectionMatrix * fragPos;
    objectPos = pos;
}