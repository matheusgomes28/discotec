#include <gl_abstractions/shaders.h>

#include <QOpenGLShaderProgram>

#include <optional>

namespace
{
    auto qogl_shader_deleter(QOpenGLShaderProgram* shader) -> void
    {
        if (shader) {
            delete shader;
        }
    }

    auto make_shader_ptr() -> OpenGLShaderProgramPtr  {
        auto* shader = new QOpenGLShaderProgram();
        return {shader, qogl_shader_deleter};
    }

    auto get_attribute_location(OpenGLShaderProgramPtr const& shader, std::string const& name) -> std::optional<int>
    {
        Q_ASSERT(shader);

        auto const attrib_location = shader->attributeLocation(name.c_str());
        return attrib_location != -1 ? std::make_optional(attrib_location) : std::nullopt;
    }
} // namespace

ShaderProgram::ShaderProgram()
    : _shader{make_shader_ptr()}
{
}

auto ShaderProgram::compile_program(ShaderProgramData const& data) -> bool {
    if (!_shader)
    {
        // definitely want to log that this
        // isn't set
        return false;
    }

    if (!_shader->addShaderFromSourceCode(QOpenGLShader::Vertex, data.vertex_shader.data()))
    {
        // Do we want to log??
        return false;
    }

    if (!_shader->addShaderFromSourceCode(QOpenGLShader::Fragment, data.fragment_shader.data()))
    {
        return false;
    }

    if (!_shader->link())
    {
        return false;
    }

    _matrix_uniform = get_matrix_uniform();
    Q_ASSERT(_matrix_uniform != -1);

    // Get the attributes
    auto const maybe_vertex_attrib = get_attribute_location(_shader, "vertex_position");
    auto const maybe_colour_attrib = get_attribute_location(_shader, "colour");
    auto const maybe_uv_attrib = get_attribute_location(_shader, "uv");
    if (!maybe_vertex_attrib ||
        !maybe_colour_attrib ||
        !maybe_uv_attrib)
    {
        return false;
    }

    _vertex_attrib = maybe_vertex_attrib.value();
    _colour_attrib = maybe_colour_attrib.value();
    _uv_attrib = maybe_uv_attrib.value();
    
    return true;
}

auto ShaderProgram::use() const -> bool
{
    if (!_shader)
    {
        return false;
    }

    return _shader->bind();
}

auto ShaderProgram::unuse() const -> bool
{
    if (!_shader)
    {
        return false;
    }

    _shader->release();
    return true;
}

auto ShaderProgram::get_id() const -> std::optional<unsigned int>
{
    if (!_shader)
    {
        return std::nullopt;
    }

    return _shader->programId();
}

// These should all be optional or something
auto ShaderProgram::get_vertex_attrib() const -> int
{
    return _vertex_attrib;
}

auto ShaderProgram::get_colour_attrib() const -> int
{
    return _colour_attrib;
}

auto ShaderProgram::get_uv_attrib() const -> int
{
    return _uv_attrib;
}

auto ShaderProgram::get_matrix_uniform() const -> int
{
    if (!_shader)
    {
        return -1;
    }

    return _shader->uniformLocation("matrix");
}

auto ShaderProgram::set_matrix(QMatrix4x4 const& matrix) -> bool
{
    if (!_shader)
    {
        return false;
    }

    _shader->setUniformValue(_matrix_uniform, matrix);
    return true;
}

auto ShaderProgram::get_shader() -> OpenGLShaderProgramPtr&
{
    return _shader;
}
