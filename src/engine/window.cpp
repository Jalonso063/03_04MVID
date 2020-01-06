#include "engine/input.hpp"
#include "engine/window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void onChangeFrameBufferSize(GLFWwindow*, const int32_t width, const int32_t height) noexcept {
    Window::instance()->setWidth(width);
    Window::instance()->setHeight(height);
    glViewport(0, 0, width, height);
    std::cout << width << " " << height << std::endl;
}

void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else {
        Input::instance()->keyPressed(key, action);
    }
}

void onMouseMove(GLFWwindow* window, double x, double y) {
    Input::instance()->mouseMoved(x, y);
}

void onScrollMove(GLFWwindow* window, double x, double y) {
    Input::instance()->scrollMove(x, y);
}

Window::Window() {
    if (!glfwInit()) {
        std::cout << "Error Initializing GLFW" << std::endl;
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_width, _height, "04MVID", nullptr, nullptr);
    if (!_window) {
        std::cout << "Error Creating Window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(_window);

    if (!gladLoadGL()) {
        std::cout << "Error Initializing GLAD" << std::endl;
        exit(-1);
    }

    glfwSetFramebufferSizeCallback(_window, onChangeFrameBufferSize);
    glfwSetKeyCallback(_window, onKeyPress);
    glfwSetCursorPosCallback(_window, onMouseMove);
    glfwSetScrollCallback(_window, onScrollMove);

    setCaptureMode(true);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::alive() const {
    return !glfwWindowShouldClose(_window);
}

void Window::frame() const {
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

bool Window::keyPressed(int key) const {
    return glfwGetKey(_window, key) == GLFW_PRESS;
}

void Window::setCaptureMode(bool toggle) const {
    if (toggle) {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Window::setHeight(uint32_t height) {
    _height = height;
    glfwSetWindowSize(_window, _width, _height);
}

void Window::setWidth(uint32_t width) {
    _width = width;
    glfwSetWindowSize(_window, _width, _height);
}