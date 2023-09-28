#ifndef GL_ABSTRACTIONS_SHADERS_H
#define GL_ABSTRACTIONS_SHADERS_H
#include <string>
#include <string_view>

constexpr std::string_view shader_code = R"(
attribute vec4 vertexPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vertexPosition;
}
)";

enum class ShaderType {
    Vertex,
    Fragment,
};

bool compile_shader(std::string const& shader_data, ShaderType type);

#endif // GL_ABSTRACTIONS_SHADERS_H
