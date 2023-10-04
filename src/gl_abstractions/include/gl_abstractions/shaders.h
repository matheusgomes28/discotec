#ifndef GL_ABSTRACTIONS_SHADERS_H
#define GL_ABSTRACTIONS_SHADERS_H

#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLShaderProgram>

#include <memory>
#include <optional>
#include <string>
#include <string_view>

QT_BEGIN_NAMESPACE
class QObject;
// class QOpenGLShaderProgram;
QT_END_NAMESPACE

using OpenGLShaderProgramDeleter = void(*)(QOpenGLShaderProgram*);
using OpenGLShaderProgramPtr = std::unique_ptr<QOpenGLShaderProgram, OpenGLShaderProgramDeleter>;

struct ShaderProgramData {
    std::string_view vertex_shader;
    std::string_view fragment_shader;
    //std::string_view geometry_shader;
};

class ShaderProgram {
public:
    ShaderProgram();
    
    bool compile_program(ShaderProgramData const&);
    bool use() const;
    bool unuse() const;

    int get_vertex_attrib() const;
    int get_colour_attrib() const;
    int get_uv_attrib() const;
    int get_matrix_uniform() const;

    bool set_matrix(QMatrix4x4 const& matrix);

    std::optional<unsigned int> get_id() const;

    // TODO : Remove testing code
    OpenGLShaderProgramPtr& get_shader();

private:
    OpenGLShaderProgramPtr _shader;
    int _vertex_attrib = -1;
    int _colour_attrib = -1;
    int _uv_attrib = -1;
    
    // TODO : Should this belog here?
    int _matrix_uniform = -1;
};

#endif // GL_ABSTRACTIONS_SHADERS_H
