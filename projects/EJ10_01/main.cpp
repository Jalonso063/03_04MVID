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
#include "engine/geometry/teapot.hpp"
#include "engine/model.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 dirLightDirection(-0.2f, 0.0f, -0.3f);

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 4.0f, 4.0f),
    glm::vec3(-4.0f, 4.0f, 0.0f)
};

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

void render(const Model& object, const Geometry& light, const Shader& s_phong, const Shader& s_light){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);


    
   

    s_light.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pointLightPositions[0]);
    model = glm::scale(model, glm::vec3(0.25f));
    s_light.set("model", model);
    s_light.set("view", view);
    s_light.set("proj", proj);
    s_light.set("lightColor", 1.0f, 1.0f, 1.0f);

    light.render();
 


    s_phong.use();

    const std::string prefixPoints = "pointLight[";
    for (uint32_t i = 0; i < pointLightPositions->length(); ++i) {
        const std::string lightName = prefixPoints + std::to_string(i) + "].";

        s_phong.set((lightName + "position").c_str(), pointLightPositions[i]);

        s_phong.set((lightName + "ambient").c_str(), 0.0f, 0.0f, 0.02f);
        s_phong.set((lightName + "diffuse").c_str(), 0.0f, 0.0f, 0.2f);
        s_phong.set((lightName + "specular").c_str(), 0.0f, 0.0f, 0.5f);
        s_phong.set((lightName + "constant").c_str(), 1.0f);
        s_phong.set((lightName + "linear").c_str(), 0.09f);
        s_phong.set((lightName + "quadratic").c_str(), 0.032f);
    }

    s_phong.set("dirLight.direction", dirLightDirection);
    s_phong.set("dirLight.ambient", 0.5f, 0.5f, 0.5f);
    s_phong.set("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
    s_phong.set("dirLight.specular", 0.5f, 0.5f, 0.5f);


    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.5f));
    s_phong.set("model", model);
    s_phong.set("view", view);
    s_phong.set("proj", proj);

    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phong.set("normalMat", normalMat);

    object.render(s_phong);
}

int main(int, char* []) {
    Window* window = Window::instance();

    glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    const Shader s_phong("../projects/EJ10_01/phong.vs", "../projects/EJ10_01/blinn.fs");
    const Shader s_light("../projects/EJ10_01/light.vs", "../projects/EJ10_01/light.fs");
    //const Model object("../assets/models/lantern/lantern_obj.obj");
    const Model object("../assets/models/plate/plate.obj");

    const Sphere sphere(1.0f, 50, 50);

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
        render(object, sphere, s_phong, s_light);
        window->frame();
    }

    return 0;
}