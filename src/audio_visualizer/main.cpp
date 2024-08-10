#include <qt_abstractions/gl_window.h>
#include <gl_abstractions/renderer.h>
#include <audio_playback/audio_file.h>

#include "shaders.h"

#include <QAudioDevice>
#include <QAudioFormat>
#include <QAudioSink>
#include <QBuffer>
#include <QCommandLineParser>
#include <QExposeEvent>
#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QVector4D>
#include <QtMath>

#include <array>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#ifdef WINDOWS
#include <QtPlugin>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin)
#endif // WINDOWS

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

    class CustomExposeEvent : public QExposeEvent
    {
    };

    class TriangleWindow : public OpenGLWindow
    {
    public:
        explicit TriangleWindow(std::shared_ptr<maecs::Registry<RenderedMesh>> registry);
        TriangleWindow() = delete;
        using OpenGLWindow::OpenGLWindow;

        void initialize() override;
        void render() override;

        void add_renderer(std::shared_ptr<Renderer> renderer);
    private:
        // Vertex stuff
        GLuint m_vertex_array = 0;
        GLuint m_vertex_buffer = 0;
        GLuint m_colour_buffer = 0;
        GLuint m_element_buffer = 0;

        // QOpenGLShaderProgram *m_program = nullptr;
        std::unique_ptr<ShaderProgram> m_program = nullptr;

        int m_frame = 0;

        std::vector<std::shared_ptr<Renderer>> _renderers;
        std::shared_ptr<maecs::Registry<RenderedMesh>> _registry;
    };

    struct ProgramArgs
    {
        // This is the file we will read and play
        std::string audio_file;
    };

    auto parse_arguments(QCoreApplication const& app) -> std::optional<ProgramArgs>
    {
        QCommandLineParser parser;
        parser.setApplicationDescription("A simple audio visualizer");
        parser.addHelpOption();
        parser.addVersionOption();

        parser.addPositionalArgument("audio", QCoreApplication::translate("main", "audio file to play"));

        // Process the actual command line arguments given by the user
        parser.process(app);

        const QStringList args = parser.positionalArguments();
        if (args.size() != 1)
        {
            return std::nullopt;
        }
        
        auto const audio_file = args[0].toStdString();
        return ProgramArgs{
            .audio_file = audio_file
        };
    }

    auto get_chunk_size(QAudioSink* sync) {
        auto const buffer_size = sync->bufferSize();
        const unsigned int target_size =  44100 / 10;

        unsigned int padding = buffer_size > target_size ? (buffer_size - target_size) : 0;
        unsigned int bytes_free = sync->bytesFree();
        if (bytes_free > padding) {
            unsigned int to_send =  bytes_free - padding;
            if (target_size < to_send) {
                return target_size;
            }
            return to_send;
        }

        return 0u;
    }

    void create_cube_mesh(std::shared_ptr<Renderer> renderer, std::shared_ptr<maecs::Registry<RenderedMesh>> registry)
    {
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

        renderer->add_mesh(triangle_mesh, registry);
    }
} // namespace

//! [2]
TriangleWindow::TriangleWindow(std::shared_ptr<maecs::Registry<RenderedMesh>> registry)
    : OpenGLWindow{}, _registry{registry}
{
}

//! [4]
void TriangleWindow::initialize()
{
}

void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();

    for (auto const& renderer : _renderers)
    {
        if (renderer)
        {
            renderer->render(retinaScale, width(), height(), screen()->refreshRate(), _registry);
        }
    }
}

void TriangleWindow::add_renderer(std::shared_ptr<Renderer> renderer)
{
    _renderers.push_back(renderer);
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    auto const maybe_program_args = parse_arguments(app);
    if (!maybe_program_args)
    {
        std::cout << "wrong arguments passed in\n";
        return -1;
    }

    // unpack all the arguments
    auto const program_args = maybe_program_args.value();

    QSurfaceFormat format;
    format.setSamples(16);

    // Contest needs to be initialized before we create
    // any other OpenGL structures (like renderers)
    auto registry = std::make_shared<maecs::Registry<RenderedMesh>>();
    TriangleWindow window{registry};
    window.setFormat(format);
    window.resize(640, 480);
    window.show();
    window.setAnimating(true);

    // Hack to make sure we have everything in place to add some
    // renderers before app.exec
    app.sendEvent(&window, new QExposeEvent{QRegion{0, 0, 640, 480}});
    app.processEvents();

    { // Creating an example renderer
        auto renderer = std::make_shared<Renderer>();
        ShaderProgramData const shader_data{
            vertexShaderSource,
            fragmentShaderSource
        };
        if (!renderer->add_shader(shader_data))
        {
            std::cout << "Could not initialize shaders\n";
        }
        window.add_renderer(renderer);
        create_cube_mesh(renderer, registry);
    }

    // example of where we can add the systems
    

    return app.exec();
}