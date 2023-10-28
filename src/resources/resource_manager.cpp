#include <cstddef>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "resource_manager.hpp"
#include "../renderer/shader_program.hpp"

resource_manager::resource_manager(const std::string& executable_path)
{
    size_t found = executable_path.find_last_of("/\\");
    m_path = executable_path.substr(0, found);
}

std::string resource_manager::get_file_string(
    const std::string& relative_file_path) const
{
    std::fstream f;
    f.open(m_path + "/" + relative_file_path.c_str(),
           std::ios::in | std::ios::binary);

    if (!f.is_open()) {
        std::cerr << "ERROR: Filed to open file: " << relative_file_path
                  << "\n";
        return std::string{};
    }

    std::stringstream buffer;
    buffer << f.rdbuf();

    return buffer.str();
}

std::shared_ptr<renderer::shader_program> resource_manager::load_shaders(
    const std::string& shader_name, std::string vertex_path,
    std::string fragment_path)
{
    std::string vertex_string = get_file_string(vertex_path);

    if (vertex_string.empty()) {
        std::cerr << "ERROR: No vertex hader!\n";
        return nullptr;
    }

    std::string fragment_string = get_file_string(fragment_path);

    if (fragment_string.empty()) {
        std::cerr << "ERROR: No fragment hader!\n";
        return nullptr;
    }

    std::shared_ptr<renderer::shader_program>& new_shader =
        m_shader_programs
            .emplace(shader_name, std::make_shared<renderer::shader_program>(
                                      vertex_string, fragment_string))
            .first->second;

    if (new_shader->is_compiled()) {
        return new_shader;
    }

    std::cerr << "Can't load shader program:\n"
              << "Vertex: " << vertex_path << "\n"
              << "Fragment: " << fragment_path << "\n";

    return nullptr;
}

std::shared_ptr<renderer::shader_program> resource_manager::get_shader(
    const std::string& shader_name)
{
    shader_programs_map::const_iterator it =
        m_shader_programs.find(shader_name);

    if (it != m_shader_programs.end()) {
        return it->second;
    }

    std::cerr << "ERROR: Can't fing the shader program: " << shader_name
              << "\n";

    return nullptr;
}