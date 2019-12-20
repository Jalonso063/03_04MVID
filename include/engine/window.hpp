#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <cstdint>

class GLFWwindow;

class Window {
    public:
        static Window* instance() {
            static Window w;
            return &w;
        }

        bool alive() const;
        void frame() const;

        void setWidth(uint32_t width) { _width = width; }
        void setHeight(uint32_t height) { _height = height; }

        uint32_t getWidth() const { return _width; }
        uint32_t getHeight() const { return _height; }

        bool keyPressed(int key) const;

        void setCaptureMode(bool toggle) const;

    private:
        Window();
        ~Window();

        Window(const Window&) = default;
        Window(Window&&) = default;
        Window& operator=(const Window&) = default;
        Window& operator=(Window&&) = default;

        GLFWwindow* _window;
        uint32_t _width = 800;
        uint32_t _height = 600;
};

#endif