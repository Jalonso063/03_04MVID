#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/camera.hpp"
#include "engine/geometry/cube.hpp"
#include "engine/input.hpp"
#include "engine/shader.hpp"
#include "engine/texture.hpp"
#include "engine/window.hpp"
#include "engine/geometry/sphere.hpp"
#include "engine/geometry/quad.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
glm::vec3 lightPos(4.0f, 1.0f, 4.0f);

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
        lastY = y;
    }

    const float xoffset = x - lastX;
    const float yoffset = lastY - y;
    lastX = x;
    lastY = y;

    camera.handleMouseMovement(xoffset, yoffset);
}

void onScrollMoved(float x, float y) {
    camera.handleMouseScroll(y);
}

void shaderRender(const Geometry& object, const Shader& shader, glm::mat4 model, glm::mat4 view, glm::mat4 proj,
                    glm::vec3 lightDiffuse, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, int shininess )
{
    shader.use();
    
    shader.set("model", model);
    shader.set("view", view);
    shader.set("proj", proj);

    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    shader.set("normalMat", normalMat);

    shader.set("viewPos", camera.getPosition());

    shader.set("material.ambient", ambient.r, ambient.g, ambient.b);
    shader.set("material.diffuse", diffuse.r, diffuse.g, diffuse.b);
    shader.set("material.specular", specular.r, specular.g, specular.b);
    shader.set("material.shininess", shininess);

    shader.set("light.position", lightPos);
    shader.set("light.ambient", 0.1f, 0.1f, 0.1f);
    shader.set("light.diffuse", lightDiffuse);
    shader.set("light.specular", 1.0f, 1.0f, 1.0f);

    object.render();
}

void render(const Geometry& object, const Geometry& light, const Shader& s_phong, const Shader& s_light) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::vec3 lightDiffuse(1.0f, 1.0f, 1.0f);

    s_light.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.25f));
    s_light.set("model", model);
    s_light.set("view", view);
    s_light.set("proj", proj);
    s_light.set("lightColor", lightDiffuse);

    light.render();

    glm::mat4 modelEmerald = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f));
    glm::vec3 ambientEmerald(0.0215f, 0.1745f, 0.0215);
    glm::vec3 diffuseEmerald(0.07568f, 0.61424f, 0.07568f);
    glm::vec3 specularEmerald(0.633f, 0.727811f, 0.633f);
    int shininessEmerald = 76; // shine de la tabla x 128
    shaderRender(object, s_phong, modelEmerald, view, proj, lightDiffuse, ambientEmerald, diffuseEmerald, specularEmerald, shininessEmerald);

    glm::mat4 modelGold = glm::mat4(1.0f);
    glm::vec3 ambientGold(0.24725f, 0.1995f, 0.0745f);
    glm::vec3 diffuseGold(0.75164f, 0.60648f, 0.22648f);
    glm::vec3 specularGold(0.628281f, 0.555802f, 0.366065f);
    int shininessGold = 51; // shine de la tabla x 128
    shaderRender(object, s_phong, modelGold, view, proj, lightDiffuse, ambientGold, diffuseGold, specularGold, shininessGold);

    glm::mat4 modelRuby = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
    glm::vec3 ambientRuby(0.1745f, 0.01175f, 0.01175f);
    glm::vec3 diffuseRuby(0.61424f, 0.04136f, 0.04136f);
    glm::vec3 specularRuby(0.727811f, 0.626959f, 0.626959f);
    int shininessRuby = 76; // shine de la tabla x 128
    shaderRender(object, s_phong, modelRuby, view, proj, lightDiffuse, ambientRuby, diffuseRuby, specularRuby, shininessRuby);
}

int main(int, char* []) {
    Window* window = Window::instance();

    glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    const Shader s_phong("../projects/EJ08_01/phong.vs", "../projects/EJ08_01/blinn.fs");
    const Shader s_light("../projects/EJ08_01/light.vs", "../projects/EJ08_01/light.fs");
    const Sphere sphere(1.0f, 50, 50);

    Texture tex("../assets/textures/blue_blocks.jpg", Texture::Format::RGB);

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
        render(sphere, sphere, s_phong, s_light);
        window->frame();
    }

    return 0;
}