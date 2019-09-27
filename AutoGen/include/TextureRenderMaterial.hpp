//
// Created by 김 범주 on 22/09/2019.
//

#ifndef DVRSTUDY_TEXTURERENDERMATERIAL_HPP
#define DVRSTUDY_TEXTURERENDERMATERIAL_HPP

#include <GLEngine/Base/Material.hpp>

class TextureRenderMaterial : public GLEngine::Material {
protected:
    GLuint m_FBO, m_buf, m_texID, m_texUnit;

public:
    explicit TextureRenderMaterial(GLuint unit, int width, int height);
    ~TextureRenderMaterial() override;

    GLuint getFBO() const noexcept;
};

#endif //DVRSTUDY_TEXTURERENDERMATERIAL_HPP
