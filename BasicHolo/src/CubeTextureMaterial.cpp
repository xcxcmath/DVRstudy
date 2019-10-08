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
        : GLEngine::Material(), m_texID(), m_texUnit(unit), m_d1Unit(unit+1),m_d2Unit(unit+2)
        ,m_data(),m_d1data(size_x*size_y*size_z),m_d2data(F0_MAX*F0_MAX){
    m_program->loadShaderFromFile("../../Resources/shader/BasicHolo/CubeTextureVertex.glsl", GL_VERTEX_SHADER);
    m_program->loadShaderFromFile("../../Resources/shader/BasicHolo/CubeTextureFragment.glsl", GL_FRAGMENT_SHADER);
    m_program->linkShader();

    std::ifstream data_stream(textureFile, std::ios::binary);
    data_stream.unsetf(std::ios::skipws);
    m_data.insert(m_data.begin(),
                  std::istream_iterator<unsigned char>(data_stream),
                  std::istream_iterator<unsigned char>());
    for(auto& v: m_data){
        if(v < 40) v = 0;
    }
    const auto x = size_x;
    const auto y = size_y;
    const auto z = size_z;
    std::vector<glm::vec3> deriv_1(x*y*z);

    auto get_voxel = [x,y,z](auto& data, int _x, int _y, int _z) {
        _x = static_cast<size_t>(glm::clamp(float(_x), 0.f, float(x-1)));
        _y = static_cast<size_t>(glm::clamp(float(_y), 0.f, float(y-1)));
        _z = static_cast<size_t>(glm::clamp(float(_z), 0.f, float(z-1)));
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
    // edge-normalization
    /*for(int i = 0; i < x; ++i){
        for(int j = 0; j < y; ++j) {
            *get_voxel(m_d1data, i, j, 0) = *get_voxel(m_d1data, i, j, 1);
            *get_voxel(m_d1data, i, j, z-1) = *get_voxel(m_d1data, i, j, z-2);
        }
    }
    for(int j = 0; j < y; ++j){
        for(int k = 0; k < z; ++k) {
            *get_voxel(m_d1data, 0, j, k) = *get_voxel(m_d1data, 1, j, k);
            *get_voxel(m_d1data, x-1, j, k) = *get_voxel(m_d1data, x-2, j, k);
        }
    }
    for(int i = 0; i < x; ++i){
        for(int k = 0; k < z; ++k) {
            *get_voxel(m_d1data, i, 0, k) = *get_voxel(m_d1data, i, 1, k);
            *get_voxel(m_d1data, i, y-1, k) = *get_voxel(m_d1data, i, y-2, k);
        }
    }*/

    float f2_max = -1000;
    std::vector<size_t> d1count(F0_MAX); std::vector<float> d1avg(F0_MAX);
    std::vector<size_t> d2count(m_d2data.size());
    for(int k = 1; k < z-1; ++k) {
        for(int j = 1; j < y-1; ++j) {
            for(int i = 1; i < x-1; ++i) {
                if(glm::length(*get_voxel(deriv_1,i,j,k)) < 0.001) continue;

                auto sd = get_gradient(m_d1data, i,j,k);
                auto grad_norm = glm::normalize(*get_voxel(deriv_1, i, j, k));
                auto f0 = static_cast<size_t>(*get_voxel(m_data,i,j,k)); // [0,255]
                auto f1 = static_cast<size_t>(*get_voxel(m_d1data,i,j,k)); // [0,255*0.9]
                ++d1count[f0]; d1avg[f0] += float(f1) / F0_MAX;
                auto f2 = glm::dot(sd, grad_norm);
                f2_max = std::max(f2, f2_max);

                m_d2data[F0_MAX * f1 + f0] += static_cast<float>(f2);
                ++d2count[F0_MAX * f1 + f0];
            }
        }
    }
    for(size_t i = 0; i < d1count.size(); ++i)if(d1count[i]) d1avg[i] /= (float(d1count[i]) / F0_MAX);
    for(size_t i = 0; i < m_d2data.size(); ++i)if(d2count[i]) m_d2data[i] /= d2count[i];
    auto d1_max = *std::max_element(d1avg.cbegin(), d1avg.cend());
    auto d2_max = *std::max_element(m_d2data.cbegin(), m_d2data.cend());
    auto d2_min = *std::min_element(m_d2data.cbegin(), m_d2data.cend());
    auto d1Scale = F0_MAX;
    auto d2Scale = (d2_max - d2_min);
    auto d2Shift = d2_min;
    for(auto& val : m_d2data) {
        val = (val - d2Shift) / d2Scale;
    }
    /*for(int i = 0; i < F0_MAX; ++i){
        for(int j = 0; j < F0_MAX; ++j){
            std::cout << m_d2data[j * F0_MAX + i] << '\t';
        } std::cout << '\n';
    }*/
    auto sigma = (d1_max / f2_max * glm::exp(-0.5f));
    std::cout << "Factor : " << d2Scale << ' ' << d2Shift << '\n';
    std::cout << "Sigma : " << sigma << '\n';

    std::transform(m_d1data.begin(), m_d1data.end(), m_d1data.begin(),
            [d1Scale](const auto& v){return v/d1Scale;});

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
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, x,y,z, 0, GL_RED, GL_FLOAT, m_d1data.data());

    glActiveTexture(GL_TEXTURE0 + m_d2Unit);
    glBindTexture(GL_TEXTURE_2D, m_texID[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, F0_MAX, F0_MAX, 0, GL_RED, GL_FLOAT, m_d2data.data());

    glUseProgram(m_program->getProgramId());
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "d0Tex"), m_texUnit);
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "d1Tex"), m_d1Unit);
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "d2Tex"), m_d2Unit);
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "backPosTex"), backTexUnit);
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "time"), 0);
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "d1Scale"), d1Scale);
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "d2Scale"), d2Scale);
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "d2Shift"), d2Shift);
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "derivSigma"), sigma);
}

CubeTextureMaterial::~CubeTextureMaterial() {
    glUseProgram(m_program->getProgramId());
    glDeleteTextures(3, m_texID);
}

void CubeTextureMaterial::updateTime(float time) {
    glUseProgram(m_program->getProgramId());
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "time"), time);
}

void CubeTextureMaterial::updateSigma(float value) {
    glUseProgram(m_program->getProgramId());
    glUniform1f(glGetUniformLocation(m_program->getProgramId(), "derivSigma"), value);
}
