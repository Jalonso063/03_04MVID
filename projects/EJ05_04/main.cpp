#include <glad/glad.h>

#include "engine/window.hpp"
#include "engine/shader.hpp"

#include "engine/texture.hpp"
#include "engine/geometry/cube.hpp"
#include "engine/geometry/quad.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

void handleInput() {/*Intentionally Left BLank*/ }

void render(const Geometry* geometries, const Shader& shader, const Texture* textures, const glm::vec3* views) {
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f); 

    glm::mat4 view = glm::mat4(1.0f);

    for (int geometryIndex = 0; geometryIndex < 4; geometryIndex++)
    {
        view = glm::translate(view, views[geometryIndex]);

        const Geometry& geom = geometries[geometryIndex];
        const Texture& tex = textures[geometryIndex];

        shader.use();

        tex.use(shader, "tex", 0);

        shader.set("model", model);
        shader.set("view", view);
        shader.set("proj", proj);

        geom.render();
    }
}

int main(int, char* []) {
    Window* window = Window::instance();

    glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    const Shader shader("../projects/EJ05_04/vertex.vs", "../projects/EJ05_04/fragment.fs");
    //const Cube cube(1.0f);
    const Quad floor(50.0f, 50.0f);
    const Cube cube1(10.0f);
    const Cube cube2(20.0f);
    const Cube cube3(20.0f);
    

    Geometry* geometries = new Geometry[4];
    geometries[0] = floor;
    geometries[1] = cube1;
    geometries[2] = cube2;
    geometries[3] = cube3;

    const Texture* textures = new Texture[4] 
    {
        Texture("../assets/textures/bricks_albedo.png", Texture::Format::RGB),
        Texture("../assets/textures/bricks_arrow.jpg", Texture::Format::RGB),
        Texture("../assets/textures/blue_blocks.jpg", Texture::Format::RGB),
        Texture("../assets/textures/cube_uv_b.jpg", Texture::Format::RGB)
    };

    const glm::vec3 views[4]
    {
        glm::vec3(0.0f, -10.0f, -50.0f),
        glm::vec3(-20.0f, -5.0f, -15.0f),
        glm::vec3(15.0f, -10.0f, -19.0f),
        glm::vec3(30.0f, -10.0f, -10.0f)
    };


    

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while (window->alive()) {
        handleInput();
        render(geometries, shader, textures, views);
        window->frame();
    }

    return 0;
}