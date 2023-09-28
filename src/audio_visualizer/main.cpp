#include <fmt/format.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>   // glfw

#include <gl_abstractions/shaders.h>

#include <iostream>
#include <memory>
#include <string>

namespace
{
    using WindowDestroyer = void(*)(GLFWwindow*);
    using WindowPtr = std::unique_ptr<GLFWwindow, WindowDestroyer>;

    auto destroy_window(GLFWwindow* window) {
        if (window) {
            glfwDestroyWindow(window);
        }
    }

    auto create_window(int width, int height, std::string const& title) -> WindowPtr {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        return {window, destroy_window};
    }

    void error_callback(int error, const char* description)
    {
        std::cout << fmt::format("Error: {}\n", description);
    }
} // namespace

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialise GLFW\n";
    }
    if (glfwSetErrorCallback(error_callback) == nullptr) {
        std::cout << "Failed to set callback\n";
    }

    auto window = create_window(640, 480, "My window");
    if (!window) {
        std::cout << "Failed to start window\n";
        return -1;
    }
    glfwMakeContextCurrent(window.get());

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialise glew\n";
        return -1;
    }

    //std::string const shader_code_str{begin(shader_code), end(shader_code)};
    //compile_shader(shader_code_str, ShaderType::Vertex);

    glfwTerminate();
}
