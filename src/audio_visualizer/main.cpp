#include <qt_abstractions/gl_window.h>
#include <gl_abstractions/renderer.h>

#include "shaders.h"

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QtMath>
#include <QVector4D>

#include <array>
#include <iostream>
#include <memory>

namespace {
    // {x, y, z}, {r, g, b}, {u, v}
    static constexpr std::array<GLfloat, 32> vertices = {
         0.0f,  0.707f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, 0.707f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    static constexpr std::array<unsigned int, 6> indices = {
        0, 1, 2, 0, 2, 3
    };

    class TriangleWindow : public OpenGLWindow
    {
    public:
        using OpenGLWindow::OpenGLWindow;

        void initialize() override;
        void render() override;

    private:
        // GLint m_posAttr = 0;
        // GLint m_colAttr = 0;
        // GLint m_matrix_uniform  = 0;

        // Vertex stuff
        GLuint m_vertex_array = 0;
        GLuint m_vertex_buffer = 0;
        GLuint m_colour_buffer = 0;
        GLuint m_element_buffer = 0;

        // QOpenGLShaderProgram *m_program = nullptr;
        std::unique_ptr<ShaderProgram> m_program = nullptr;

        int m_frame = 0;


        std::unique_ptr<Renderer> m_renderer;
    };

    void show_gl_logs(TriangleWindow& window) {
        auto logger = window.getLogger();

        const QList<QOpenGLDebugMessage> messages = logger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            qDebug() << message;
        }
    }
} // namespace

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    TriangleWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}
//! [2]

//! [4]
void TriangleWindow::initialize()
{
    if (!m_renderer)
    {
        m_renderer = std::make_unique<Renderer>();
    }

    ShaderProgramData const shader_data{
        vertexShaderSource,
        fragmentShaderSource
    };
    
    if (!m_renderer->add_shader(shader_data))
    {
        std::cout << "Could not initialize shaders\n";
    }

    Mesh triangle_mesh{
        .data = {
            0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 0
            0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 1
            0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 2
            0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 3
            0.5f,  0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 4
            0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 5
            0.0f,  0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 6
            0.0f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 7
        },
        .indices = {
            0, 1, 2, // face 1
            1, 3, 2, // face 1
            1, 4, 3, // face 2
            4, 5, 3, // face 2
            4, 6, 7, // face 3
            4, 5, 7, // face 3
            0, 2, 6, // face 4
            2, 6, 7, // face 4
            2, 7, 3, // face 5
            3, 5, 7, // face 5
            0, 1, 6, // face 6
            1, 4, 6,
        },
        .shader_program = 0
    };
    m_renderer->add_mesh(triangle_mesh);

    /*
    auto const shader_program_data = ShaderProgramData{
        .vertex_shader = vertexShaderSource,
        .fragment_shader = fragmentShaderSource
    };

    m_program = std::make_unique<ShaderProgram>();
    m_program->compile_program(shader_program_data);

    show_gl_logs(*this);

    // Uploading vertex data to openG
    glGenBuffers(1, &m_vertex_buffer);
    Q_ASSERT(m_vertex_buffer > 0); // Is this valid?
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Upload the elements
    glGenBuffers(1, &m_element_buffer);
    Q_ASSERT(m_element_buffer > 0); // Is this valid?
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Bind the VAO - data config book
    glGenVertexArrays(1, &m_vertex_array);
    Q_ASSERT(m_vertex_array > 0); // Is this valid?
    glBindVertexArray(m_vertex_array);
    show_gl_logs(*this);

    // needs to be enabled before the vertex bufers?

    // enable the vertices data stuff
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glEnableVertexAttribArray(m_program->get_vertex_attrib()); // Is this correct?
    glVertexAttribPointer(m_program->get_vertex_attrib(), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);

    show_gl_logs(*this);

    // enable the colour data stuff
    glEnableVertexAttribArray(m_program->get_colour_attrib()); // Is this correct?
    glVertexAttribPointer(m_program->get_colour_attrib(), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));

    show_gl_logs(*this);
    */
}
//! [4]

//! [5]
void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    m_renderer->render(retinaScale, width(), height(), screen()->refreshRate());
    /*

    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->use();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
    
    m_program->set_matrix(matrix);

    show_gl_logs(*this);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer);


    glBindVertexArray(m_vertex_array);
    glEnableVertexAttribArray(m_program->get_vertex_attrib());
    glEnableVertexAttribArray(m_program->get_colour_attrib());

    show_gl_logs(*this);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);

    show_gl_logs(*this);

    m_program->unuse();

    ++m_frame;
    */
}
