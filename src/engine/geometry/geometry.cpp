#include "engine/geometry/geometry.hpp"

#include <glad/glad.h>

Geometry::~Geometry() {
    glDeleteBuffers(4, _VBO);
    glDeleteVertexArrays(1, &_VAO);
}

void Geometry::uploadData(const float* positions, const float* uvs, const float* normals,
    const uint32_t* indices) {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(4, _VBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _VBO[0]);         //elements
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * _nElements, indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO[1]);                 //positions
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _nVertices * 3, positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO[2]);                 //uvs
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _nVertices * 2, uvs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO[3]);                 //normals
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _nVertices * 3, normals, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Geometry::render() const {
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, _nElements, GL_UNSIGNED_INT, 0);
}