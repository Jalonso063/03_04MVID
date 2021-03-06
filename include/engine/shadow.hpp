#ifndef __SHADOW_H__
#define __SHADOW_H__
#include <engine\shader.hpp>

class Shadow
{
public:
    Shadow();
    ~Shadow();

    void render() const;
    void bindTexture(const Shader& shader) const;

private:

    uint32_t _fbo = 0;
    uint32_t _depthMap = 0;
};

#endif

