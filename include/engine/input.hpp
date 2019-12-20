#ifndef __INPUT_H__
#define __INPUT_H__

#include <functional>
#include "engine/window.hpp"

class Input {
public:
    static Input* instance() {
        static Input i;
        return &i;
    }

    static bool isKeyPressed(int key) {
        return Window::instance()->keyPressed(key);
    }

    void setKeyPressedCallback(const std::function<void(int, int)>& f) {
        _keyPressedCallback = f;
    }

    void setMouseMoveCallback(const std::function<void(float, float)>& f) {
        _mouseMoveCallback = f;
    }
    void setScrollMoveCallback(const std::function<void(float, float)>& f) {
        _scrollMoveCallback = f;
    }

    void keyPressed(int key, int action) const {
        if (_keyPressedCallback) _keyPressedCallback(key, action);
    }

    void mouseMoved(float x, float y) const {
        if (_mouseMoveCallback) _mouseMoveCallback(x, y);
    }

    void scrollMove(float x, float y) const {
        if (_scrollMoveCallback) _scrollMoveCallback(x, y);
    }

private:
    std::function<void(int, int)> _keyPressedCallback = nullptr;
    std::function<void(float, float)> _mouseMoveCallback = nullptr;
    std::function<void(float, float)> _scrollMoveCallback = nullptr;

};

#endif