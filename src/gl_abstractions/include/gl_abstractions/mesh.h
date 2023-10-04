#ifndef GL_ABSTRACTIONS_MESH_H
#define GL_ABSTRACTIONS_MESH_H
#include <vector>

struct Mesh {
    /// @brief this contains all the drawing data, layed
    /// out as {x, y, z, r, g, b, u, v}, where
    /// - {x, y, z} represents the vertex position
    /// - {r, g, b} represents the colour data
    /// - {u, v} represent the texture coordinates
    std::vector<float> data;

    /// @brief The indices to be drawns. Each three
    /// indices forms a triangle drawn with the OpenGL
    /// `glDrawElements(GL_TRIANGLES, ..., GL_UNSIGNED_INT)`
    std::vector<unsigned int> indices;

    /// The id of the shader program used to draw this mesh
    unsigned int shader_program;
};

#endif // GL_ABSTRACTIONS_MESH_H