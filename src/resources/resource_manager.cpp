#include <cstddef>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "resource_manager.hpp"
#include "../renderer/shader_program.hpp"
#include "../renderer/texture_2D.hpp"
#include "../renderer/sprite.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

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

std::shared_ptr<renderer::texture_2D> resource_manager::load_texture(
    const std::string& texture_name, const std::string& texture_path)
{
    int chanels = 0;
    int width = 0;
    int height = 0;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* pixels =
        stbi_load(std::string(m_path + "/" + texture_path).c_str(), &width,
                  &height, &chanels, 0);

    if (!pixels) {
        std::cerr << "ERROR: Can't load image: " << texture_path << "\n";
        return nullptr;
    }

    std::shared_ptr<renderer::texture_2D> new_texture =
        m_textures
            .emplace(texture_name, std::make_shared<renderer::texture_2D>(
                                       width, height, pixels, chanels,
                                       GL_NEAREST, GL_CLAMP_TO_EDGE))
            .first->second;

    stbi_image_free(pixels);

    return new_texture;
}

std::shared_ptr<renderer::texture_2D> resource_manager::get_texture(
    const std::string& texture_name)
{
    texture_2D_map::const_iterator it = m_textures.find(texture_name);

    if (it != m_textures.end()) {
        return it->second;
    }

    std::cerr << "ERROR: Can't fing the texture: " << texture_name << "\n";

    return nullptr;
}

std::shared_ptr<renderer::sprite> resource_manager::load_sprite(
    const std::string& sprite_name, const std::string& texture_name,
    const std::string& shader_name, const unsigned int sprite_widht,
    const unsigned int sprite_height)
{
    auto texture_ptr = get_texture(texture_name);
    if (!texture_ptr) {
        std::cerr << "ERROR: Can't fing the texture: " << texture_name
                  << " for the sprite: " << sprite_name << "\n";
    }

    auto shader_ptr = get_shader(shader_name);
    if (!shader_ptr) {
        std::cerr << "ERROR: Can't fing the shader: " << shader_name
                  << " for the sprite: " << sprite_name << "\n";
    }

    auto sprite_ptr =
        m_sprites
            .emplace(texture_name,
                     std::make_shared<renderer::sprite>(
                         texture_ptr, shader_ptr, glm::vec2(0.0f, 0.0f),
                         glm::vec2(sprite_widht, sprite_height)))
            .first->second;
    
    return sprite_ptr;
}

std::shared_ptr<renderer::sprite> resource_manager::get_sprite(
    const std::string& sprite_name)
{
    sprite_map::const_iterator it = m_sprites.find(sprite_name);

    if (it != m_sprites.end()) {
        return it->second;
    }

    std::cerr << "ERROR: Can't fing the sprite: " << sprite_name << "\n";

    return nullptr;
}