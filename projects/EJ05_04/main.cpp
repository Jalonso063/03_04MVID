#include <glad/glad.h>

#include "engine/window.hpp"
#include "engine/shader.hpp"

#include "engine/texture.hpp"
#include "engine/geometry/cube.hpp"
#include "engine/geometry/quad.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

const int cubesInWorld = 3;

void handleInput() {/*Intentionally Left BLank*/ }

void render(const Geometry& floor, Geometry* cubes, const Shader& shader, const Texture& floorTexture, const Texture* textures, const glm::vec3* models, const glm::vec3* views) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    shader.use();

    model = glm::translate(model, glm::vec3(0.0f, -5.0f, -25.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));

    floorTexture.use(shader, "tex", 0);

    shader.set("model", model);
    shader.set("view", view);
    shader.set("proj", proj);

    floor.render();


    for (int geometryIndex = 0; geometryIndex < cubesInWorld; geometryIndex++)
    {

        model = glm::mat4(1.0f);
        model = glm::translate(model, models[geometryIndex]);
        view = glm::mat4(1.0f);
        view = glm::translate(view, views[geometryIndex]);

        const Geometry& geom = cubes[geometryIndex];
        const Texture& tex = textures[geometryIndex];

        shader.use();

        tex.use(shader, "tex", 0);

        shader.set("model", model);
        shader.set("view", view);

        geom.render();
    }
}

int main(int, char* []) {
    Window* window = Window::instance();

    glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    const Shader shader("../projects/EJ05_04/vertex.vs", "../projects/EJ05_04/fragment.fs");
    //const Cube cube(1.0f);

    const Quad floor(1.0f);
    Texture floorTexture("../assets/textures/bricks_albedo.png", Texture::Format::RGB);


    const Cube cube1(5.0f);
    const Cube cube2(7.0f);
    const Cube cube3(10.0f);


    Geometry* cubes = new Geometry[cubesInWorld];
    cubes[0] = cube1;
    cubes[1] = cube2;
    cubes[2] = cube3;

    const Texture* cubeTextures = new Texture[cubesInWorld]
    {
        Texture("../assets/textures/bricks_arrow.jpg", Texture::Format::RGB),
        Texture("../assets/textures/blue_blocks.jpg", Texture::Format::RGB),
        Texture("../assets/textures/cube_uv_b.jpg", Texture::Format::RGB)
    };

    const glm::vec3 cubeModels[cubesInWorld]
    {
        glm::vec3(0.0f, -2.5f, -10.0f),
        glm::vec3(15.0f, -1.5f, -25.0f),
        glm::vec3(-20.0f, 0.0f, -35.0f)
    };

    const glm::vec3 cubeViews[cubesInWorld]
    {
        glm::vec3(0.0f, 0.0f, -10.0f),
        glm::vec3(15.0f, 0.0f, -25.0f),
        glm::vec3(-20.0f, 0.0f, -35.0f)
    };

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    while (window->alive()) {
        handleInput();
        render(floor, cubes, shader, floorTexture, cubeTextures, cubeModels, cubeViews);
        window->frame();
    }

    return 0;
}