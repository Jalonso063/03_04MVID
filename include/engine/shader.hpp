#ifndef __SHADER_H__
#define __SHADER_H__

#include <cstdint>
#include <string>
#include <glm/glm.hpp>

class Shader {
    enum class Type {
        Vertex,
        Fragment,
        Geometry,
        Program
    };

    public:
        Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
        Shader() = delete;
        ~Shader();

        Shader(const Shader&) = default;
        Shader(Shader&&) = default;
        Shader& operator=(const Shader&) = default;
        Shader& operator=(Shader&&) = default;

        void use() const;

        void set(const char* name, int value) const;
        void set(const char* name, float value) const;
        void set(const char* name, bool value) const;
        void set(const char* name, float value1, float value2) const;
        void set(const char* name, float value1, float value2, float value3) const;
        void set(const char* name, float value1, float value2, float value3, float value4) const;
        void set(const char* name, const glm::vec2& value) const;
        void set(const char* name, const glm::vec3& value) const;
        void set(const char* name, const glm::vec4& value) const;
        void set(const char* name, const glm::mat2& value) const;
        void set(const char* name, const glm::mat3& value) const;
        void set(const char* name, const glm::mat4& value) const;

    private:
        static void loadShader(const char* path, std::string* code);
        static void checkErrors(uint32_t shader, Type type);
        uint32_t id_;
};


#endif