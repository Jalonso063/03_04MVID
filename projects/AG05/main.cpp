#include <glad/glad.h>

#include "engine/input.hpp"
#include "engine/window.hpp"
#include "engine/shader.hpp"

#include <iostream>

#include "engine/texture.hpp"
#include "engine/geometry/quad.hpp"

void handleInput() {
    std::vector<std::pair<int, int>> keys = Input::instance()->getKeys();
    for (auto& key : keys) {
        std::cout << key.first << " - " << key.second << std::endl;
    }
}

void render(const Geometry& geom, const Shader& shader, Texture& tex1, Texture& tex2) {
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    tex1.use(shader, "tex_1", 0);
    tex2.use(shader, "tex_2", 1);

    geom.render();
}

int main(int, char* []) {
    Window* window = Window::instance();

    glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    const Shader shader("../projects/AG05/vertex.vs", "../projects/AG05/fragment.fs");
    const Quad quad(1.0f);

    Texture tex1("../assets/textures/bricks_arrow.jpg", Texture::Format::RGB);
    Texture tex2("../assets/textures/blue_blocks.jpg", Texture::Format::RGB);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while (window->alive()) {
        handleInput();
        render(quad, shader, tex1, tex2);
        window->frame();
    }

    return 0;
}