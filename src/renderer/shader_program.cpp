#include "shader_program.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace renderer {
bool shader_program::create_shader(const std::string& source,
                                   const GLenum shader_type, GLuint& shader_ID)
{
    shader_ID = glCreateShader(shader_type);
    const char* code = source.c_str();
    glShaderSource(shader_ID, 1, &code, nullptr);
    glCompileShader(shader_ID);

    GLint success;
    glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info_log[1024];
        glGetShaderInfoLog(shader_ID, 1024, nullptr, info_log);
        std::cerr << "ERROR::SHADER: Compile time error:\n" << info_log << "\n";
        return false;
    }

    return true;
}

shader_program::shader_program(const std::string& vertex_shader,
                               const std::string& fragment_shader)
{
    GLuint vertex_shader_ID;
    if (!create_shader(vertex_shader, GL_VERTEX_SHADER, vertex_shader_ID)) {
        std::cerr << "ERROR::SHADER: Vertex shader compile time error.\n";
        return;
    }

    GLuint fragment_shader_ID;
    if (!create_shader(fragment_shader, GL_FRAGMENT_SHADER,
                       fragment_shader_ID)) {
        std::cerr << "ERROR::SHADER: Fragment shader compile time error.\n";
        glDeleteShader(vertex_shader_ID);
        return;
    }

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex_shader_ID);
    glAttachShader(m_ID, fragment_shader_ID);
    glLinkProgram(m_ID);

    GLint success;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

    if (!success) {
        GLchar info_log[1024];
        glGetShaderInfoLog(m_ID, 1024, nullptr, info_log);
        std::cerr << "ERROR::SHADER: Link time error:\n" << info_log << "\n";
    } else {
        m_is_compiled = true;
    }

    glDeleteShader(vertex_shader_ID);
    glDeleteShader(fragment_shader_ID);
}

shader_program& shader_program::operator=(shader_program&& shader_prog)
{
    glDeleteProgram(m_ID);
    m_ID = shader_prog.m_ID;
    m_is_compiled = shader_prog.m_is_compiled;

    shader_prog.m_ID = 0;
    shader_prog.m_is_compiled = false;

    return *this;
}

shader_program::shader_program(shader_program&& shader_prog)
{
    m_ID = shader_prog.m_ID;
    m_is_compiled = shader_prog.m_is_compiled;

    shader_prog.m_ID = 0;
    shader_prog.m_is_compiled = false;
}

shader_program::~shader_program()
{
    glDeleteProgram(m_ID);
}

void shader_program::use() const
{
    glUseProgram(m_ID);
}

void shader_program::set_int(const std::string& name, const GLint value)
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void shader_program::set_matrix_4(const std::string& name, glm::mat4& matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(matrix));
}

}  // namespace renderer