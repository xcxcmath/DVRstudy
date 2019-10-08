//
// Created by 김 범주 on 29/09/2019.
//

#ifndef DVRSTUDY_TEXCOORDRENDERMATERIAL_HPP
#define DVRSTUDY_TEXCOORDRENDERMATERIAL_HPP

#include <GLEngine/Base/Material.hpp>

class TexCoordRenderMaterial : public GLEngine::Material {
protected:
    GLuint m_FBO, m_buf, m_texID, m_texUnit;

public:
    explicit TexCoordRenderMaterial(GLuint unit, int width, int height);
    ~TexCoordRenderMaterial() override;

    GLuint getFBO() const noexcept;
};

#endif //DVRSTUDY_TEXCOORDRENDERMATERIAL_HPP
