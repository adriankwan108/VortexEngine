#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoor;

/*
layout(set = 0, binding = 0) uniform Camera {
    mat4 view;
    mat4 proj;
} camera_VP;
*/

layout(location = 0) out vec3 fragColor;
/* layout(location = 1) out vec2 fragTexCoor; */

void main() {
    /* gl_Position = camera_VP.proj * camera_VP.view * vec4(inPosition, 0.0, 1.0); */
    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
    /* fragTexCoor = inTexCoor; */
}