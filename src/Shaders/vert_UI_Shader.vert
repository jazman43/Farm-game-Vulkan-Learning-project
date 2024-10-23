#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

layout(push_constant) uniform PushConstants {
    mat4 projection;
} pushConstants;

void main() {
    gl_Position = pushConstants.projection * vec4(inPosition, 0.0, 1.0);
    fragTexCoord = inTexCoord;
}