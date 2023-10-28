#pragma once

#include <glad/glad.h>

#include <string>

namespace renderer {
class shader_program {
    bool m_is_compiled = false;
    GLuint m_ID = 0;

    bool create_shader(const std::string& source, const GLenum shader_type,
                       GLuint& shader_ID);

public:
    shader_program() = delete;

    shader_program(shader_program&) = delete;

    shader_program& operator=(const shader_program&) = delete;

    shader_program(const std::string& vertex_shader,
                   const std::string& fragment_shader);

    shader_program& operator=(shader_program&& shader_prog);

    shader_program(shader_program&& shader_prog);

    ~shader_program();

    bool is_compiled() const
    {
        return m_is_compiled;
    }

    void use() const;

    void set_int(const std::string& name, const GLint value);
};
}  // namespace renderer