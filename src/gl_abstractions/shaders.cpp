#include <gl_abstractions/shaders.h>

#include <GL/glew.h>

namespace {
    auto get_gl_shader_type(ShaderType type) {
        switch (type) {
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        }

        return GL_VERTEX_SHADER;
    }
} // namespace

bool compile_shader(std::string const& shader_data, ShaderType type) {

    auto const gl_shader_type = get_gl_shader_type(type);
    
    auto const shader_id = glCreateShader(gl_shader_type);

    if (shader_id == 0) {
        // TODO : log did not return a shader
        return false;
    }

    auto const source_ptr = shader_data.c_str();
    
    // These need error handling
    glShaderSource(shader_id, 1, &source_ptr, NULL);
    glCompileShader(shader_id);

    return true;
}
