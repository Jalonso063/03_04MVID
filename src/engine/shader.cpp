#include "engine/shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    std::string vertexCode, fragmentCode, geometryCode;
    loadShader(vertexPath, &vertexCode);
    loadShader(fragmentPath, &fragmentCode);
    if (geometryPath) {
        loadShader(geometryPath, &geometryCode);
    }

    const char* vertexStr = vertexCode.c_str();
    const uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexStr , nullptr);
    glCompileShader(vertex);
    checkErrors(vertex, Type::Vertex);

    const char* fragmentStr = fragmentCode.c_str();
    const uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentStr, nullptr);
    glCompileShader(fragment);
    checkErrors(fragment, Type::Fragment);

    uint32_t geometry = 0;
    if (geometryPath) {
        const char* geometryStr = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometryStr, nullptr);
        glCompileShader(geometry);
        checkErrors(geometry, Type::Geometry);
    }

    id_ = glCreateProgram();
    glAttachShader(id_, vertex);
    glAttachShader(id_, fragment);
    if (geometryPath) {
        glAttachShader(id_, geometry);
    }

    glLinkProgram(id_);
    checkErrors(id_, Type::Program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath) {
        glDeleteShader(geometry);
    }
}

Shader::~Shader() {
    glDeleteProgram(id_);
}

void Shader::use() const {
    glUseProgram(id_);
}

void Shader::loadShader(const char* path, std::string* code) {
    std::ifstream file;

    file.open(path, std::ios_base::in);
    if (file) {
        std::stringstream stream;
        stream << file.rdbuf();
        *code = stream.str();
        file.close();
    } else {
        std::cout << "Error in load shader " << path << std::endl;
    }
}

void Shader::checkErrors(uint32_t shader, Type type) {
    int success;
    char infoLog[512];
    if (type != Type::Program) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "Error Compiling Shader " << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "Error Linking Program " << infoLog << std::endl;
        }
    }
}

void Shader::set(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(id_, name), value);
}

void Shader::set(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(id_, name), static_cast<int>(value));
}

void Shader::set(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(id_, name), value);
}

void Shader::set(const char* name, float value1, float value2) const {
    glUniform2f(glGetUniformLocation(id_, name), value1, value2);
}

void Shader::set(const char* name, float value1, float value2, float value3) const {
    glUniform3f(glGetUniformLocation(id_, name), value1, value2, value3);
}

void Shader::set(const char* name, float value1, float value2, float value3, float value4) const {
    glUniform4f(glGetUniformLocation(id_, name), value1, value2, value3, value4);
}

void Shader::set(const char* name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(id_, name), 1, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(id_, name), 1, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(id_, name), 1, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::mat2& value) const {
    glUniformMatrix2fv(glGetUniformLocation(id_, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::mat3& value) const {
    glUniformMatrix3fv(glGetUniformLocation(id_, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, GL_FALSE, glm::value_ptr(value));
}
