#version 330 core
in vec4 fragPos;
in vec4 projected;
in vec3 objectPos;
out vec4 outputColor;

uniform mat4 worldTransform;
uniform mat4 cameraTransform;
uniform sampler3D tex;
uniform sampler1D deriv1Tex;
uniform sampler1D deriv2Tex;
uniform sampler2D backPosTex;
uniform float derivShift;
uniform float derivSigma;
uniform float time;

#define PI 3.14159265358979323846264338
#define stepsize 0.014

float intensity_factor(vec3 rayPos) {
    //return clamp(-303 + 306 * sin(rayPos.z * 6 + time * 2), 0.2, 3);
    //return clamp(-203 + 206 * cos((rayPos.z + sin(time) / 2.5 + 0.5) * 2 * PI), 0.2, 3);
    return 1;
}

float opacity_factor(vec3 rayPos) {
    //return clamp(-303 + 307 * sin(length(rayPos - vec3(0.5,0.4,0.5)) * 6 - time * 3), 0.2, 4);
    //return clamp(-303 + 307 * sin(rayPos.x * 6 + time * 2), 0.1, 4);
    return 1;
}

float transfer_opacity(float intensity) {
    float g = texture(deriv1Tex, intensity).r + derivShift;
    float h = texture(deriv2Tex, intensity).r + derivShift;
    //if(abs(g) < 0.00001) return 0;
    float p = -h / g * pow(derivSigma, 2);
    return pow((1 - 1*clamp(abs(p), 0, 1/1.f)), 1);
}

vec3 transfer_color(float intensity) {
    float g = texture(deriv1Tex, intensity).r + derivShift;
    float h = texture(deriv2Tex, intensity).r + derivShift;
    float gh = -h / g;
    //if(abs(g) < 0.00001) return 0;
    vec3 p = vec3(pow(derivSigma,2), pow(derivSigma * 0.9,1), pow(derivSigma * 1.1, 2)) * gh;
    vec3 ret = vec3(1,1,1) - vec3(clamp(abs(p.x)/100,0,1),clamp(abs(p.y)/200,0,1),clamp(abs(p.z)/300,0,1));
    return ret;
}

vec3 color_output(vec3 in_color, float intensity, vec3 rayPos) {
    vec3 ret;
    float A = transfer_opacity(intensity) * opacity_factor(rayPos);
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
    if(length(frontPos - backPos) >= stepsize){
        for (float i = 0; i <= 1.74; i += stepsize) {
            float intensity = texture(tex, ray_pos).r;
            ray_pos += ray_dir * stepsize;
            if (front_back_len < length(ray_pos - backPos)) break;
            //if (intensity < 0.02) continue;
            //intensity *= intensity_factor(ray_pos);
            outputColor.rgb = color_output(outputColor.rgb, intensity, ray_pos);
        }
    }

    return;
}
