#ifndef SHADERS_H
#define SHADERS_H

static constexpr const char *vertexShaderSource = R"(
#version 420 core

layout(location = 0) in vec3 vertex_position; // Input position
layout(location = 1) in vec3 colour;    // Input color
layout(location = 2) in vec2 uv;    // Uv tex coords

out vec4 fragColour; // Output color for the fragment shader

uniform mat4 matrix;

void main()
{
    double unused = dot(uv, uv);
    //double unused2 = dot(colour, colour);
    gl_Position = matrix * vec4(vertex_position, 1.0); // Set the position
    fragColour = vec4(colour, 1.0 * unused); // Pass the color to the fragment shader
    //fragColour = vec4(1.0, 0.0 + 0.00001 * unused, 0.0000001 * unused2, 1.0);
}
)";

static constexpr const char *fragmentShaderSource = R"(
#version 420
in vec4 fragColour;
out vec4 colour;

void main() {
  colour = fragColour;
})";

#endif // SHADERS_H
