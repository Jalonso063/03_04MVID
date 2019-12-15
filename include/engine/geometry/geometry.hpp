#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__ 1
#include <cstdint>
#include <glad/glad.h>

//Attributes
//0 - positions
//1 - uvs
//2 - normals

class Geometry {
    public:
        Geometry() = default;

        virtual ~Geometry() {
            glDeleteBuffers(4, _VBO);
            glDeleteVertexArrays(1, &_VAO);
        }

        Geometry(const Geometry&) = default;
        Geometry& operator=(const Geometry&) = default;

        Geometry(Geometry&&) noexcept = default;
        Geometry& operator=(Geometry&&) noexcept = default;

        virtual void render() const;

    protected:
        void uploadData(const float* positions, const float* uvs,
            const float* normals, const uint32_t* indices);
    protected:
        uint32_t _VAO = 0;
        uint32_t _VBO[4] { 0,0,0,0 };
        uint32_t _nVerts = 0;

};
#endif