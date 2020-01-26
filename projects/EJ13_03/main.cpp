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
#include <iostream>

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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

std::tuple<uint32_t, uint32_t, uint32_t> createFBO() {
    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    uint32_t textureColor;
    glGenTextures(1, &textureColor);
    glBindTexture(GL_TEXTURE_2D, textureColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::instance()->getWidth(), Window::instance()->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColor, 0);

    uint32_t rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Window::instance()->getWidth(), Window::instance()->getHeight());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer Incomplete" << std::endl;
    }

    return std::make_tuple(fbo, textureColor, rbo);
}

void render(const Geometry& quad, const Geometry& cube, const Shader& s_phong, const Shader& s_fbo,
    const Texture& t_albedo, const Texture& t_specular, const uint32_t fboVert, const uint32_t fboVert_texture,
    const uint32_t fboHor, const uint32_t fboHor_texture) {
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), 0.1f, 100.0f);

    //FIRST PASS
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, fboVert);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    s_phong.use();
    s_phong.set("view", view);
    s_phong.set("proj", proj);

    s_phong.set("viewPos", camera.getPosition());

    s_phong.set("light.position", lightPos);
    s_phong.set("light.ambient", 0.1f, 0.1f, 0.1f);
    s_phong.set("light.diffuse", 0.5f, 0.5f, 0.5f);
    s_phong.set("light.specular", 1.0f, 1.0f, 1.0f);

    t_albedo.use(s_phong, "material.diffuse", 0);
    t_specular.use(s_phong, "material.specular", 1);
    s_phong.set("material.shininess", 32);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    s_phong.set("model", model);
    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phong.set("normalMat", normalMat);

    quad.render();

    model = glm::mat4(1.0f);
    s_phong.set("model", model);
    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phong.set("normalMat", normalMat);

    cube.render();

    //SECOND PASS


    glBindFramebuffer(GL_FRAMEBUFFER, fboHor);
    glDisable(GL_DEPTH_TEST);

    // primera pasada de blur gausiano, pasada vertical

    s_fbo.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboVert_texture);
    s_fbo.set("offset", 1.0f / (Window::instance()->getWidth()), 0.0f);
    s_fbo.set("screenTexture", 0);
    quad.render();

    // segunda pasada de blur gausiano, pasada horizontal

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, fboHor_texture);
    s_fbo.set("offset", 0.0f, 1.0f / (Window::instance()->getHeight()));
    quad.render();
}

int main(int, char* []) {
    Window* window = Window::instance();

    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    const Shader s_phong("../projects/EJ13_03/phong.vs", "../projects/EJ13_03/blinn.fs");
    const Shader s_fbo("../projects/EJ13_03/fbo.vs", "../projects/EJ13_03/fbo.fs");
    const Texture t_albedo("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
    const Texture t_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
    const Quad quad(2.0f);
    const Cube cube(1.0f);

    auto fboVert = createFBO();
    auto fboHor = createFBO();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Input::instance()->setKeyPressedCallback(onKeyPress);
    Input::instance()->setMouseMoveCallback(onMouseMoved);
    Input::instance()->setScrollMoveCallback(onScrollMoved);

    while (window->alive()) {
        const float currentFrame = glfwGetTime();
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handleInput(deltaTime);
        render(quad, cube, s_phong, s_fbo, t_albedo, t_specular,
            std::get<0>(fboVert), std::get<1>(fboVert),
            std::get<0>(fboHor), std::get<1>(fboHor));
        window->frame();
    }

    glDeleteFramebuffers(1, &std::get<0>(fboVert));
    glDeleteTextures(1, &std::get<1>(fboVert));
    glDeleteRenderbuffers(1, &std::get<2>(fboVert));

    glDeleteFramebuffers(1, &std::get<0>(fboHor));
    glDeleteTextures(1, &std::get<1>(fboHor));
    glDeleteRenderbuffers(1, &std::get<2>(fboHor));

    return 0;
}