#ifndef __CUBE_H__
#define __CUBE_H__ 1

#include "geometry.hpp"

class Cube final : public Geometry {
    public:
        Cube() = delete;
        explicit Cube(float size);
        explicit Cube(float width, float height);

    private:
        float _size;
        float _width;
        float _height;
};
#endif