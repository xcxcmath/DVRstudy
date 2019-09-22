//
// Created by bjk on 19. 9. 21..
//

#ifndef DVRSTUDY_VOLUMETEXTUREMATERIAL_HPP
#define DVRSTUDY_VOLUMETEXTUREMATERIAL_HPP

#include <GLEngine/Base/Material.hpp>

class VolumeTextureMaterial : public GLEngine::Material {
private:
    GLuint m_texID;
    std::vector<char> m_data;

public:
    explicit VolumeTextureMaterial(const std::string& textureFile, size_t x, size_t y, size_t z);
    virtual ~VolumeTextureMaterial();
};

#endif //DVRSTUDY_VOLUMETEXTUREMATERIAL_HPP
