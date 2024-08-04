#ifndef GL_ABSTRACTIONS_RENDERER_H
#define GL_ABSTRACTIONS_RENDERER_H
#include <gl_abstractions/mesh.h>
#include <gl_abstractions/shaders.h>

#include <maecs/maecs.hpp>

#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLShaderProgram>

#include <memory>

struct RenderedMesh
{
    /// @brief `Mesh` data to be drawn
    Mesh mesh;
    /// @brief The OpenGL buffer ID
    unsigned int buffer;
    /// @brief the VAO id for this mesh
    unsigned int vertex_array;
    /// @brief the element buffers
    unsigned int element_buffer;
};

class Renderer : protected QOpenGLFunctions_4_2_Core
{ 
public:

    Renderer();

    /// @brief Creates a shader program with the give GLSL
    /// code for vertex and fragment shaders.
    /// @param shader_data is the shader code to be added
    /// @return true if a new shader was created and added to
    /// this renderer, false otherwise.
    bool add_shader(ShaderProgramData const& shader_data);

    // TODO: should this be forward declared?
    /// @brief Adds a mesh to the list of meshes to be rendered
    /// by this object. See the `Mesh` documentation for
    /// more information.
    /// @param mesh is a mesh object containing the draw data
    /// @param registry the ecs
    void add_mesh(Mesh const& mesh, std::shared_ptr<maecs::Registry<RenderedMesh>> registry);

    /// This function renders all the meshes previously
    /// added to this class.
    /// TODO : docs for retina scale
    void render(double retina_scale, int width, int height, double refresh_rate, std::shared_ptr<maecs::Registry<RenderedMesh>> registry);

private:
    std::map<unsigned int, ShaderProgram> _shaders;
    // std::vector<RenderedMesh> _meshes;

    // TODO : Make each mesh have their own transform
    int _matrix_uniform = -1;
    

    int _frame = 0;
};

#endif // GL_ABSTRACTIONS_RENDERER_H
