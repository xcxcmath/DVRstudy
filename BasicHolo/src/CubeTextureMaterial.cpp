//
// Created by bjk on 19. 9. 29..
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <tuple>
#include <CubeTextureMaterial.hpp>

CubeTextureMaterial::CubeTextureMaterial(GLuint unit, GLuint backTexUnit, const std::string &textureFile, size_t size_x,
        size_t size_y, size_t size_z)
        : GLEngine::Material(), m_texID(), m_texUnit(unit),m_d2Unit(unit+2)
        ,m_data(),m_d1data(size_x*size_y*size_z),m_d2data(f0_max*f1_max){
    // TODO : scale

    m_program->loadShaderFromFile("../../Resources/shader/BasicHolo/CubeTextureVertex.glsl", GL_VERTEX_SHADER);
    m_program->loadShaderFromFile("../../Resources/shader/BasicHolo/CubeTextureFragment.glsl", GL_FRAGMENT_SHADER);
    m_program->linkShader();

    std::ifstream data_stream(textureFile, std::ios::binary);
    data_stream.unsetf(std::ios::skipws);

    m_data.insert(m_data.begin(),
                  std::istream_iterator<unsigned char>(data_stream),
                  std::istream_iterator<unsigned char>());
    const auto x = size_x;
    const auto y = size_y;
    const auto z = size_z;
    std::vector<glm::vec3> deriv_1(x*y*z);

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
                *get_voxel(m_d1data, i, j, k) = glm::length(*get_voxel(deriv_1,i,j,k));
            }
        }
    }

    float max_f2 = -1000;
    for(int k = 1; k < z-1; ++k) {
        for(int j = 1; j < y-1; ++j) {
            for(int i = 1; i < x-1; ++i) {
                if(glm::length(*get_voxel(deriv_1,i,j,k)) < 0.001) continue;

                auto sd = get_gradient(m_d1data, i,j,k);
                auto grad_norm = glm::normalize(*get_voxel(deriv_1, i, j, k));
                auto f0 = static_cast<size_t>(*get_voxel(m_data,i,j,k)); // [0,255]

                auto f1 = static_cast<size_t>(glm::floor(*get_voxel(m_d1data, i,j, k)) + 128.f);
                auto f2 = glm::dot(sd, grad_norm);
                max_f2 = std::max(f2, max_f2);

                m_d2data[f0_max * f1 + f0] += static_cast<float>(f2) / (f0_max * 3.1f);
            }
        }
    }
    auto max_f1 = *std::max_element(m_d1data.cbegin(), m_d1data.cend());
    auto d2_max = *std::max_element(m_d2data.cbegin(), m_d2data.cend());
    auto d2_min = *std::min_element(m_d2data.cbegin(), m_d2data.cend());
    for(auto& val : m_d2data) {
        val = (val - d2_min) / (d2_max - d2_min);
    }
    auto sigma = (max_f1 / max_f2 * glm::exp(-0.5f));
    std::cout << "Sigma : " << sigma << '\n';

    for(auto& val: m_d1data) {
        val /= 255.f;
    }

    glGenTextures(3, m_texID);
    glActiveTexture(GL_TEXTURE0 + m_texUnit);
    glBindTexture(GL_TEXTURE_3D, m_texID[0]);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, x, y, z, 0, GL_RED, GL_UNSIGNED_BYTE, m_data.data());

    glActiveTexture(GL_TEXTURE0 + m_d1Unit);
    glBindTexture(GL_TEXTURE_3D, m_texID[1]);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, x,y,z, 0, GL_RED, GL_FLOAT, m_d1data.data());

    glActiveTexture(GL_TEXTURE0 + m_d2Unit);
    glBindTexture(GL_TEXTURE_2D, m_texID[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, f0_max, f1_max, 0, GL_RED, GL_FLOAT, m_d2data.data());

    glUseProgram(m_program->getProgramId());
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "d0Tex"), m_texUnit);
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "d1Tex"), m_d1Unit);
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "d2Tex"), m_d2Unit);
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "backPosTex"), backTexUnit);
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "time"), 0);
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "derivShift"), d2_min);
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "derivSigma"), sigma);
}
