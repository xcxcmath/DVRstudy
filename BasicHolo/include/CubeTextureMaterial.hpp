//
// Created by bjk on 19. 9. 29..
//

#ifndef DVRSTUDY_CUBETEXTUREMATERIAL_HPP
#define DVRSTUDY_CUBETEXTUREMATERIAL_HPP

#include <GLEngine/Base/Material.hpp>

class CubeTextureMaterial : public GLEngine::Material {
private:
    GLuint m_texUnit;
    GLuint m_d1Unit;
    GLuint m_d2Unit;
    GLuint m_texID[3];
    std::vector<unsigned char> m_data;
    std::vector<float> m_d1data;
    std::vector<float> m_d2data;
    static constexpr GLuint F0_MAX = 256;

public:
    explicit CubeTextureMaterial(GLuint unit, GLuint backTexUnit,
            const std::string& textureFile, size_t size_x, size_t size_y, size_t size_z);
    ~CubeTextureMaterial() override;

    void updateTime(float time);
    void updateSigma(float value);
};

#endif //DVRSTUDY_CUBETEXTUREMATERIAL_HPP
