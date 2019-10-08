#version 330 core
in vec2 uv;
out vec4 outputColor;

uniform sampler2D tex;

void main() {
    outputColor = texture(tex, uv).rgba;
}
