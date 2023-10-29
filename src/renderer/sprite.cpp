#include "sprite.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "shader_program.hpp"
#include "texture_2D.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace renderer {

sprite::sprite(const std::shared_ptr<texture_2D> texture_ptr,
               const std::shared_ptr<shader_program> shader_program_ptr,
               const glm::vec2& position, const glm::vec2& size,
               const float rotation)
    : m_texture_ptr(std::move(texture_ptr)),
      m_shader_program_ptr(std::move(shader_program_ptr)),
      m_position(position),
      m_size(size),
      m_rotation(rotation)
{
    // clang-format off
    const GLfloat vertex_coords[] = {
        // X  Y
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,

        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };

    const GLfloat tex_coords[] = {
        // U  V
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,

        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };
    // clang-format on

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_vertex_coords_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_coords_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_coords), &vertex_coords,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &m_texture_coords_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_texture_coords_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), &tex_coords,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

sprite::~sprite()
{
    glDeleteBuffers(1, &m_vertex_coords_VBO);
    glDeleteBuffers(1, &m_texture_coords_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void sprite::render() const
{
    m_shader_program_ptr->use();
    glm::mat4 model(1.0f);

    model = glm::translate(model, glm::vec3(m_position, 0.0f));

    model = glm::translate(model,
                           glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.0f));

    model = glm::rotate(model, glm::radians(m_rotation),
                        glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::translate(model,
                           glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.0f));

    model = glm::scale(model, glm::vec3(m_size, 1.0f));

    glBindVertexArray(m_VAO);
    m_shader_program_ptr->set_matrix_4("model_mat", model);
    glActiveTexture(GL_TEXTURE0);
    m_texture_ptr->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void sprite::set_position(const glm::vec2& position)
{
    m_position = position;
}

void sprite::set_size(const glm::vec2& size)
{
    m_size = size;
}

void sprite::set_rotation(const float rotation)
{
    m_rotation = rotation;
}

}  // namespace renderer