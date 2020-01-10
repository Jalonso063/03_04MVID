#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/camera.hpp"
#include "engine/geometry/cube.hpp"
#include "engine/geometry/quad.hpp"
#include "engine/input.hpp"
#include "engine/shader.hpp"
#include "engine/texture.hpp"
#include "engine/window.hpp"
#include "engine/geometry/sphere.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse = true;

void handleInput(float dt) {
    Input* input = Input::instance();

    if (input->isKeyPressed(GLFW_KEY_W)) {
        camera.handleKeyboard(Camera::Movement::Forward, dt);
    }
    if (input->isKeyPressed(GLFW_KEY_S)) {
        camera.handleKeyboard(Camera::Movement::Backward, dt);
    }
    if (input->isKeyPressed(GLFW_KEY_A)) {
        camera.handleKeyboard(Camera::Movement::Left, dt);
    }
    if (input->isKeyPressed(GLFW_KEY_D)) {
        camera.handleKeyboard(Camera::Movement::Right, dt);
    }
}

void onKeyPress(int key, int action) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        Window::instance()->setCaptureMode(true);
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        Window::instance()->setCaptureMode(false);
    }
}

void onMouseMoved(float x, float y) {
    if (firstMouse) {
        firstMouse = false;
        lastX = x;
        lastY = 0;
    }

    const float xoffset = x - lastX;
    const float yoffset = lastY - 0;
    lastX = x;
    lastY = 0;

    camera.handleMouseMovement(xoffset, yoffset);
}

void onScrollMoved(float x, float y) {
    camera.handleMouseScroll(y);
}

void render(const Geometry* geometries, const Shader& shader, const Texture* textures, const glm::vec3* views) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f);

    for (int geometryIndex = 0; geometryIndex < 4; geometryIndex++)
    {
        glm::mat4 view = glm::translate(camera.getViewMatrix(), views[geometryIndex]);

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

    const Shader shader("../projects/EJ06_02/vertex.vs", "../projects/EJ06_02/fragment.fs");
    //const Cube cube(1.0f);
    //const Sphere sphere(1.0f, 50, 50);

    const Quad floor(50.0f, 50.0f);
    const Cube cube1(10.0f);
    const Cube cube2(20.0f);
    const Cube cube3(5.0f);


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
        glm::vec3(-20.0f, -5.0f, -40.0f),
        glm::vec3(0.0f, 0.0f, -80.0f),
        glm::vec3(30.0f, -7.5f, -70.0f)
    };

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Input::instance()->setKeyPressedCallback(onKeyPress);
    Input::instance()->setMouseMoveCallback(onMouseMoved);
    Input::instance()->setScrollMoveCallback(onScrollMoved);

    while (window->alive()) {
        const float currentFrame = glfwGetTime();
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handleInput(deltaTime);
        render(geometries, shader, textures, views);
        window->frame();
    }

    return 0;
}