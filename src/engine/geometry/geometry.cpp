#include "engine/geometry/geometry.hpp"

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/detail/func_geometric.inl>

Geometry::~Geometry() {
    glDeleteBuffers(4, _VBO);
    glDeleteVertexArrays(1, &_VAO);
}

void Geometry::calcTangents(const float* positions, const float* uvs, const float* normals, float* tangents, float* biTangents) const {
    for (size_t i = 0; i < _nVertices; i += 3) {
        const uint32_t idx3 = i * 3;
        const uint32_t idx2 = i * 2;

        glm::vec3 v0(positions[idx3 + 0], positions[idx3 + 1], positions[idx3 + 2]);
        glm::vec3 v1(positions[idx3 + 3], positions[idx3 + 4], positions[idx3 + 5]);
        glm::vec3 v2(positions[idx3 + 6], positions[idx3 + 7], positions[idx3 + 8]);

        glm::vec2 uv0(uvs[idx2 + 0], uvs[idx2 + 1]);
        glm::vec2 uv1(uvs[idx2 + 2], uvs[idx2 + 3]);
        glm::vec2 uv2(uvs[idx2 + 4], uvs[idx2 + 5]);

        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        glm::vec2 deltaUv1 = uv1 - uv0;
        glm::vec2 deltaUv2 = uv2 - uv0;

        float r = 1 / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUv2.y - deltaPos2 * deltaUv1.y) * r;
        glm::vec3 biTangent = (deltaPos2 * deltaUv1.x - deltaPos1 * deltaUv2.x) * r;

        tangents[idx3 + 0] = tangents[idx3 + 3] = tangents[idx3 + 6] = tangent.x;
        tangents[idx3 + 1] = tangents[idx3 + 4] = tangents[idx3 + 7] = tangent.y;
        tangents[idx3 + 2] = tangents[idx3 + 5] = tangents[idx3 + 8] = tangent.z;

        biTangents[idx3 + 0] = biTangents[idx3 + 3] = biTangents[idx3 + 6] = biTangent.x;
        biTangents[idx3 + 1] = biTangents[idx3 + 4] = biTangents[idx3 + 7] = biTangent.y;
        biTangents[idx3 + 2] = biTangents[idx3 + 5] = biTangents[idx3 + 8] = biTangent.z;
    }

    for (size_t i = 0; i < _nVertices; ++i) {
        const uint32_t idx3 = i * 3;

        glm::vec3 n(normals[idx3 + 0], normals[idx3 + 1], normals[idx3 + 2]);
        glm::vec3 t(tangents[idx3 + 0], tangents[idx3 + 1], tangents[idx3 + 2]);
        glm::vec3 b(biTangents[idx3 + 0], biTangents[idx3 + 1], biTangents[idx3 + 2]);

        t = glm::normalize(t - n * glm::dot(t, n));

        if (glm::dot(glm::cross(n, t),b) < 0.0f) {
            t = t * -1.0f;
        }

        tangents[idx3 + 0] = t.x;
        tangents[idx3 + 1] = t.y;
        tangents[idx3 + 2] = t.z;
    }
}


void Geometry::uploadData(const float* positions, const float* uvs, const float* normals,
    const uint32_t* indices) {
    const size_t length = _nVertices * 3;
    const auto tangents = new float[length];
    const auto biTangents = new float[length];

    calcTangents(positions, uvs, normals, tangents, biTangents);

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(6, _VBO);

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

    glBindBuffer(GL_ARRAY_BUFFER, _VBO[4]);                 //tangents
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _nVertices * 3, tangents, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO[5]);                 //bitangents
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _nVertices * 3, biTangents, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete[] tangents;
    delete[] biTangents;
}

void Geometry::render() const {
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, _nElements, GL_UNSIGNED_INT, 0);
}