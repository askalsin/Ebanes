#pragma once

#include <string>
#include <memory>
#include <map>

namespace renderer {
class shader_program;
class texture_2D;
class sprite;
}  // namespace renderer

class resource_manager {
    typedef std::map<const std::string,
                     std::shared_ptr<renderer::shader_program>>
        shader_programs_map;

    shader_programs_map m_shader_programs;

    typedef std::map<const std::string, std::shared_ptr<renderer::texture_2D>>
        texture_2D_map;

    texture_2D_map m_textures;

    typedef std::map<const std::string, std::shared_ptr<renderer::sprite>>
        sprite_map;

    sprite_map m_sprites;

    std::string m_path;

    std::string get_file_string(const std::string& relative_file_path) const;

public:
    resource_manager(const std::string& executable_path);

    ~resource_manager() = default;

    resource_manager(const resource_manager&) = delete;

    resource_manager& operator=(const resource_manager&) = delete;

    resource_manager& operator=(const resource_manager&&) = delete;

    resource_manager(const resource_manager&&) = delete;

    std::shared_ptr<renderer::shader_program> load_shaders(
        const std::string& shader_name, std::string vertex_path,
        std::string fragment_path);

    std::shared_ptr<renderer::shader_program> get_shader(
        const std::string& shader_name);

    std::shared_ptr<renderer::texture_2D> load_texture(
        const std::string& texture_name, const std::string& texture_path);

    std::shared_ptr<renderer::texture_2D> get_texture(
        const std::string& texture_name);

    std::shared_ptr<renderer::sprite> load_sprite(
        const std::string& sprite_name, const std::string& texture_name,
        const std::string& shader_name, const unsigned int sprite_widht,
        const unsigned int sprite_height);

    std::shared_ptr<renderer::sprite> get_sprite(
        const std::string& sprite_name);
};