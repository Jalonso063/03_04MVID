#ifndef __FBO_H__
#define __FBO_H__ 1

#include <cstdint>
#include <engine\shader.hpp>


class Fbo 
{
public:
    Fbo();
    ~Fbo();


    void render() const;
    void bindTexture(const Shader& shader) const;

private:

    uint32_t _fbo = 0;
    uint32_t _textureColor = 0;
    uint32_t _rbo = 0;
    
};

#endif
