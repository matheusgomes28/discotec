#include <gl_abstractions/renderer.h>
#include <gl_abstractions/mesh.h>
#include <gl_abstractions/shaders.h>

#include <QMatrix4x4>

#include <utility>

Renderer::Renderer()
{
    initializeOpenGLFunctions();
}

auto Renderer::add_mesh(Mesh const& mesh) -> void
{
    unsigned int buffer_id = 0;

    // Uploading vertex data to openG
    glGenBuffers(1, &buffer_id);
    Q_ASSERT(buffer_id > 0); // Is this valid?

    /// Set the data information
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, mesh.data.size() * sizeof(float), mesh.data.data(), GL_STATIC_DRAW);

    // create and set element data
    unsigned int element_buffer = 0;
    glGenBuffers(1, &element_buffer);
    Q_ASSERT(element_buffer > 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

    unsigned int vertex_array_object = 0;
    glGenVertexArrays(1, &vertex_array_object);
    Q_ASSERT(vertex_array_object > 0);
    glBindVertexArray(vertex_array_object);

    glBindBuffer(GL_VERTEX_ARRAY, buffer_id);

    // TODO : Enable the vertices
    // TODO : Enable the colour
    // TODO : Enable the UV
    Q_ASSERT(_shaders.size() > 0); // TODO: need to attach a mesh to a shader
    auto const& shader = _shaders[1]; // THIS NEEDS TO CHANGE!!! Do we always have 1 as the ID?
    auto const vertex_attrib = shader.get_vertex_attrib();
    Q_ASSERT(vertex_attrib != -1);
    auto const colour_attrib = shader.get_colour_attrib();
    Q_ASSERT(colour_attrib != -1);
    auto const uv_attrib = shader.get_uv_attrib();
    Q_ASSERT(uv_attrib != -1);

    // The main data buffer
    glEnableVertexAttribArray(vertex_attrib); // Is this correct?
    glVertexAttribPointer(vertex_attrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
    glEnableVertexAttribArray(colour_attrib);
    glVertexAttribPointer(colour_attrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(uv_attrib);
    glVertexAttribPointer(uv_attrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));


    RenderedMesh const rendered_mesh{
        std::move(mesh),
        buffer_id,
        vertex_array_object,
        element_buffer
    };
    _meshes.push_back(std::move(rendered_mesh));
}

auto Renderer::add_shader(ShaderProgramData const& shader_data) -> bool
{
    ShaderProgram shader_program{};
    
    if (!shader_program.compile_program(shader_data))
    {
        return false;
    }

    // Adds to opengl
    auto const id = shader_program.get_id();
    if (id == std::nullopt)
    {
        return false;
    }
    
    _shaders[id.value()] = std::move(shader_program);
    // _shaders.insert(std::pair{id.value(), std::move(shader_program)});
    return true;
}

auto Renderer::render(double retinaScale, int width, int height, double refresh_rate) -> void
{
    glViewport(0, 0, width * retinaScale, height * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    // TODO : Make it work for all renderers
    Q_ASSERT(_shaders.size() > 0);
    auto& shader = _shaders[1]; // THIS ALSO NEEDS TO CHANGE
    shader.use();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.translate(-0.25f, -0.25f, 0.0f);
    matrix.rotate(100.0f * _frame / refresh_rate, 1, 0, 0);
    Q_ASSERT(shader.set_matrix(matrix));


    for (auto const& mesh : _meshes)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mesh.buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.element_buffer);
        glBindVertexArray(mesh.vertex_array);

        // TODO : these don't need to be here
        auto const vertex_attrib = shader.get_vertex_attrib();
        Q_ASSERT(vertex_attrib != -1);
        auto const colour_attrib = shader.get_colour_attrib();
        Q_ASSERT(colour_attrib != -1);
        auto const uv_attrib = shader.get_uv_attrib();
        Q_ASSERT(uv_attrib != -1);

        glEnableVertexAttribArray(vertex_attrib);
        glEnableVertexAttribArray(colour_attrib);
        glEnableVertexAttribArray(uv_attrib);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.mesh.indices.size()), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    shader.unuse();

    ++_frame;
}
