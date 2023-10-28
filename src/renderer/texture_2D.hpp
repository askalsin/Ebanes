#pragma once

#include <glad/glad.h>
#include <string>

namespace renderer {
class texture_2D {
    GLuint m_ID;
    GLenum m_mode;
    unsigned int m_width, m_height;

public:
    texture_2D(const GLuint width, const GLuint height,
               const unsigned char* data, const unsigned int channels = 4,
               const GLenum filter = GL_LINEAR,
               const GLenum wrap_mode = GL_CLAMP_TO_EDGE);

    texture_2D() = delete;

    texture_2D(const texture_2D&) = delete;

    texture_2D& operator=(const texture_2D&) = delete;

    texture_2D& operator=(texture_2D&& texture_2d);

    texture_2D(texture_2D&& texture_2d);

    ~texture_2D();

    void bind() const;
};
}  // namespace renderer
