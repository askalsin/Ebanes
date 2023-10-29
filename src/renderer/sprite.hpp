#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <memory>

namespace renderer {

class texture_2D;
class shader_program;

class sprite {
    std::shared_ptr<texture_2D> m_texture_ptr;
    std::shared_ptr<shader_program> m_shader_program_ptr;
    glm::vec2 m_position;
    glm::vec2 m_size;
    float m_rotation;
    GLuint m_VAO;
    GLuint m_vertex_coords_VBO;
    GLuint m_texture_coords_VBO;

public:
    sprite(const std::shared_ptr<texture_2D> texture_ptr,
           const std::shared_ptr<shader_program> shader_program_ptr,
           const glm::vec2& position = glm::vec2(0.0f),
           const glm::vec2& size = glm::vec2(1.0f),
           const float rotation = 0.0f);

    ~sprite();

    sprite(const sprite&) = delete;

    sprite& operator=(const sprite&) = delete;

    void render() const;

    void set_position(const glm::vec2& position);

    void set_size(const glm::vec2& size);

    void set_rotation(const float rotation);
};

}  // namespace renderer