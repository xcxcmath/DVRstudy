#version 330 core
in vec4 fragPos;
in vec4 projected;
in vec3 objectPos;
out vec4 outputColor;

uniform mat4 worldTransform;
uniform mat4 cameraTransform;
uniform sampler3D d0Tex;
uniform sampler3D d1Tex;
uniform sampler2D d2Tex;
uniform sampler2D backPosTex;
uniform float d1Scale;
uniform float d2Shift;
uniform float d2Scale;
uniform float derivSigma;
uniform float time;

#define PI 3.14159265358979323846264338
#define stepsize 0.02

float transfer_opacity(vec3 texPos) {
    float f = texture(d0Tex, texPos).r;
    float g = texture(d1Tex, texPos).r * d1Scale;
    float h = texture(d2Tex, vec2(f, g / d1Scale)).r * d2Scale + d2Shift;
    //if(abs(g) < 0.00001) return 0;
    float p = -h / g * pow(derivSigma, 2);
    return pow((1 - 1*clamp(abs(p * 1000), 0, 1/1.f)), 5) / 5;
}

vec3 color_output(vec3 in_color, vec3 rayPos) {
    vec3 ret;
    float A = transfer_opacity(rayPos);// * opacity_factor(rayPos);
    //float A = intensity * intensity;
    //vec3 C = vec3(0.5, 0.75, 1);
    //vec3 C = transfer_color(intensity);
    vec3 C = vec3(0.5, 0.75, 1);
    ret = (1 - A) * in_color + A * C;

    return ret;
}

void main() {
    outputColor = vec4(0, 0, 0, 1);

    vec3 frontPos = (objectPos + vec3(1)) / 2;
    vec3 backPos = texture(backPosTex, ((projected.xy / projected.w) + vec2(1)) / 2).rgb;
    float front_back_len = length(frontPos - backPos);

    vec3 ray_pos = backPos;
    vec3 ray_dir = normalize(frontPos - backPos);
    float i = 0;
    if(length(frontPos - backPos) >= stepsize){
        for (i = 0; i <= 1.74; i += stepsize) {
            ray_pos += ray_dir * stepsize;
            if (front_back_len < length(ray_pos - backPos)) break;
            outputColor.rgb = color_output(outputColor.rgb, ray_pos);
        }
    }

    //outputColor.a = step(0, length(outputColor.rgb));
    //if(length(outputColor.rgb) < 0.001) outputColor.a = 0;
}
