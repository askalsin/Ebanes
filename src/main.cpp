#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>

#include "renderer/shader_program.hpp"
#include "renderer/texture_2D.hpp"
#include "renderer/sprite.hpp"
#include "resources/resource_manager.hpp"

// clang-format off
GLfloat point[] = {
     0.0f,  50.f, 0.0f,
     50.f, -50.f, 0.0f,
    -50.f, -50.f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat tex_coord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
};
// clang-format on

glm::ivec2 window_size(640, 480);

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    window_size.x = width;
    window_size.y = height;
    glViewport(0, 0, window_size.x, window_size.y);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action,
                     int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(int argc, char** argv)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "Library initialization failed!\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_size.x, window_size.y, "Ebanes", nullptr,
                              nullptr);
    if (!window) {
        std::cout << "Failed to create a window!\n";
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "Can't load GLAD!\n";
        return -1;
    }

    std::cout << "Render: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glad_glGetString(GL_VERSION) << "\n";

    glClearColor(1, 1, 0, 1);

    {
        resource_manager res_manager(argv[0]);

        auto default_shader_program = res_manager.load_shaders(
            "default_shader", "resources/shaders/vertex.txt",
            "resources/shaders/fragment.txt");

        if (!default_shader_program) {
            std::cerr << "ERROR: Can't create shader program: default_shader\n";
            return -1;
        }

        auto sprite_shader_program = res_manager.load_shaders(
            "sprite_shader", "resources/shaders/vertex_sprite.txt",
            "resources/shaders/fragment_sprite.txt");

        if (!sprite_shader_program) {
            std::cerr << "ERROR: Can't create sprite shader program: "
                      << "sprite_shader\n";
            return -1;
        }

        auto tex = res_manager.load_texture("default_texture",
                                            "resources/textures/map_16x16.png");

        auto sprite_ptr = res_manager.load_sprite(
            "new_sprite", "default_texture", "sprite_shader", 50, 100);

        sprite_ptr->set_position(glm::vec2(300, 100));

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint tex_coord_vbo = 0;
        glGenBuffers(1, &tex_coord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, tex_coord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord), tex_coord,
                     GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, tex_coord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        default_shader_program->use();
        default_shader_program->set_int("tex", 0);

        glm::mat4 model_matrix1 = glm::mat4(1.0f);
        model_matrix1 =
            glm::translate(model_matrix1, glm::vec3(100.0f, 50.0f, 0.0f));

        glm::mat4 model_matrix2 = glm::mat4(1.0f);
        model_matrix2 =
            glm::translate(model_matrix2, glm::vec3(590.0f, 50.0f, 0.0f));

        glm::mat4 projection_matrix =
            glm::ortho(0.0f, static_cast<float>(window_size.x), 0.0f,
                       static_cast<float>(window_size.y), -100.0f, 100.f);

        default_shader_program->set_matrix_4("projection_mat",
                                             projection_matrix);

        sprite_shader_program->use();
        sprite_shader_program->set_int("tex", 0);

        sprite_shader_program->set_matrix_4("projection_mat",
                                            projection_matrix);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            default_shader_program->use();

            glBindVertexArray(vao);

            tex->bind();

            default_shader_program->set_matrix_4("model_mat", model_matrix1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            default_shader_program->set_matrix_4("model_mat", model_matrix2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            sprite_ptr->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}