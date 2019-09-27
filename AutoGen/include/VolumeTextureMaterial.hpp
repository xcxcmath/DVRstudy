//
// Created by bjk on 19. 9. 21..
//

#ifndef DVRSTUDY_VOLUMETEXTUREMATERIAL_HPP
#define DVRSTUDY_VOLUMETEXTUREMATERIAL_HPP

#include <GLEngine/Base/Material.hpp>

class VolumeTextureMaterial : public GLEngine::Material {
private:
    GLuint m_texUnit;
    GLuint m_deriv1Unit, m_deriv2Unit;
    GLuint m_texID[3];
    std::vector<unsigned char> m_data;
    std::vector<float> m_deriv1data, m_deriv2data;

public:
    explicit VolumeTextureMaterial(GLuint unit, GLuint backTexUnit, const std::string& textureFile, size_t x, size_t y, size_t z);
    ~VolumeTextureMaterial() override;

    void updateTime(float time);
    void updateSigma(float value);
};

#endif //DVRSTUDY_VOLUMETEXTUREMATERIAL_HPP
