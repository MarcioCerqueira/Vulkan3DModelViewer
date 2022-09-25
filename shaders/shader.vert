#version 450

layout(binding = 0) uniform ModelViewProjectionTransformation {
    mat4 model;
    mat4 view;
    mat4 projection;
} mvp;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = mvp.projection * mvp.view * mvp.model * vec4(inPosition, 1.0);
    fragNormal = inNormal;
    fragTexCoord = inTexCoord;
}