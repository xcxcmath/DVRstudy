//
// Created by bjk on 19. 9. 21..
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <VolumeTextureMaterial.hpp>

VolumeTextureMaterial::VolumeTextureMaterial(GLuint unit, GLuint backTexUnit, const std::string &textureFile, size_t x, size_t y, size_t z)
: GLEngine::Material(), m_texID(), m_texUnit(unit), m_deriv1Unit(unit+1),m_deriv2Unit(unit+2),
m_data(), m_deriv1data(256), m_deriv2data(256){
    m_program->loadShaderFromFile("../../Resources/shader/AutoGenVertex.glsl", GL_VERTEX_SHADER);
    m_program->loadShaderFromFile("../../Resources/shader/AutoGenFragment.glsl", GL_FRAGMENT_SHADER);
    m_program->linkShader();

    std::ifstream data_stream(textureFile, std::ios::binary);
    data_stream.unsetf(std::ios::skipws);

    m_data.insert(m_data.begin(),
            std::istream_iterator<unsigned char>(data_stream),
            std::istream_iterator<unsigned char>());

    std::vector<glm::vec3> deriv_1(x*y*z);
    std::vector<float> deriv_norm(x*y*z); // 0 ~ 256

    auto get_voxel = [x,y,z](auto& data, int _x, int _y, int _z) {
        if(_x < 0) _x = 0; if(_x >= x) _x = x-1;
        if(_y < 0) _y = 0; if(_y >= y) _y = y-1;
        if(_z < 0) _z = 0; if(_z >= z) _z = z-1;
        return &(data[_z * (x*y) + _y * (x) + _x]);
    };
    auto get_near_voxel = [&get_voxel](auto& data, int _x, int _y, int _z, int offset) {
        return glm::vec3(
                static_cast<float>(*get_voxel(data, _x+offset, _y, _z)),
                static_cast<float>(*get_voxel(data, _x, _y+offset, _z)),
                static_cast<float>(*get_voxel(data, _x, _y, _z+offset))
                );
    };
    auto get_gradient = [&get_voxel, &get_near_voxel](auto& data, int _x, int _y, int _z) {
        return (get_near_voxel(data,_x,_y,_z,1)-get_near_voxel(data,_x,_y,_z,-1))/2.f;
    };

    for(int k = 1; k < z-1; ++k) {
        for(int j = 1; j < y-1; ++j) {
            for(int i = 1; i < x-1; ++i) {
                *get_voxel(deriv_1, i, j, k) = get_gradient(m_data, i, j, k);
                *get_voxel(deriv_norm, i, j, k) = glm::length(*get_voxel(deriv_1,i,j,k));
            }
        }
    }

    const float plane_sz = (256*glm::sqrt(3.f)) * (256*3);
    float max_f1 = -1000, max_f2 = -1000;
    for(int k = 1; k < z-1; ++k) {
        for(int j = 1; j < y-1; ++j) {
            for(int i = 1; i < x-1; ++i) {
                if(glm::length(*get_voxel(deriv_1,i,j,k)) < 0.001) continue;

                auto sd = get_gradient(deriv_norm, i,j,k);
                auto grad_norm = glm::normalize(*get_voxel(deriv_1, i, j, k));
                auto f0 = *get_voxel(m_data,i,j,k);
                auto f1 = *get_voxel(deriv_norm, i,j, k);
                auto f2 = glm::dot(sd, grad_norm);
                max_f1 = std::max(f1, max_f1);
                max_f2 = std::max(f2, max_f2);

                m_deriv1data[f0] += static_cast<float>(f1) / plane_sz;
                m_deriv2data[f0] += static_cast<float>(f2) / plane_sz;
            }
        }
    }
    auto deriv1_max = *std::max_element(m_deriv1data.cbegin(), m_deriv1data.cend());
    auto deriv2_max = *std::max_element(m_deriv2data.cbegin(), m_deriv2data.cend());
    auto deriv1_min = *std::min_element(m_deriv1data.cbegin(), m_deriv1data.cend());
    auto deriv2_min = *std::min_element(m_deriv2data.cbegin(), m_deriv2data.cend());
    auto deriv_max = std::max(deriv1_max, deriv2_max);
    auto deriv_min = std::min(deriv1_min, deriv2_min);
    auto deriv_shift = deriv_min;
    for(int i = 0; i < 256; ++i){
        m_deriv1data[i] = (m_deriv1data[i] - deriv_shift) / (deriv_max - deriv_min);
        m_deriv2data[i] = (m_deriv2data[i] - deriv_shift) / (deriv_max - deriv_min);
        std::cout << m_deriv1data[i] << ' ' << m_deriv2data[i] << '\n';
    }
    std::cout << "Sigma : " << (max_f1 / max_f2 * glm::exp(-0.5f)) << '\n';

    glGenTextures(3, m_texID);
    glActiveTexture(GL_TEXTURE0 + m_texUnit);
    glBindTexture(GL_TEXTURE_3D, m_texID[0]);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, x, y, z, 0, GL_RED, GL_UNSIGNED_BYTE, m_data.data());

    glActiveTexture(GL_TEXTURE0 + m_deriv1Unit);
    glBindTexture(GL_TEXTURE_1D, m_texID[1]);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RED, 256, 0, GL_RED, GL_FLOAT, m_deriv1data.data());

    glActiveTexture(GL_TEXTURE0 + m_deriv2Unit);
    glBindTexture(GL_TEXTURE_1D, m_texID[2]);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RED, 256, 0, GL_RED, GL_FLOAT, m_deriv2data.data());

    glUseProgram(m_program->getProgramId());
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "tex"), m_texUnit);
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "deriv1Tex"), m_deriv1Unit);
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "deriv2Tex"), m_deriv2Unit);
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "backPosTex"), backTexUnit);
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "time"), 0);
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "derivShift"), deriv_shift);
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "derivSigma"), 0.98);
}

VolumeTextureMaterial::~VolumeTextureMaterial() {
    glUseProgram(m_program->getProgramId());
    glDeleteTextures(3, m_texID);
}

void VolumeTextureMaterial::updateTime(float time) {
    glUseProgram(m_program->getProgramId());
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "time"), time);
}

void VolumeTextureMaterial::updateSigma(float value) {
    glUseProgram(m_program->getProgramId());
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "derivSigma"), value);
}

