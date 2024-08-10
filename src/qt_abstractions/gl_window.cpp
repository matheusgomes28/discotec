/// This code was acquired from the Qt documentation
/// at https://doc.qt.io/qt-5.15/qtgui-openglwindow-example.html
/// It is under the BSD license, see page for more details.

#include <qt_abstractions/gl_window.h>
#include <qt_abstractions/moc_gl_window.cpp>

#include <QOpenGLContext>
#include <QOpenGLDebugLogger>
#include <QOpenGLPaintDevice>
#include <QPainter>

OpenGLWindow::OpenGLWindow(QWindow *parent)
    : QWindow(parent)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

OpenGLWindow::~OpenGLWindow()
{
    delete m_device;
}

void OpenGLWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

void OpenGLWindow::initialize()
{
}

void OpenGLWindow::render()
{
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size() * devicePixelRatio());
    m_device->setDevicePixelRatio(devicePixelRatio());

    QPainter painter(m_device);
    render(&painter);
}

void OpenGLWindow::renderLater()
{
    requestUpdate();
}

bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void OpenGLWindow::renderNow()
{
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        
        // This creates Gl2 by default
        auto surface = requestedFormat();
        surface.setVersion(4, 2);
        surface.setProfile(QSurfaceFormat::CoreProfile);
        surface.setOption(QSurfaceFormat::DebugContext);

        setFormat(surface);
        m_context->setFormat(surface);
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);
    // m_logger = new QOpenGLDebugLogger(this);
    // m_logger->initialize();

    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

    render();

    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

void OpenGLWindow::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}

// QOpenGLDebugLogger* OpenGLWindow::getLogger()
// {
//     return m_logger;
// }
