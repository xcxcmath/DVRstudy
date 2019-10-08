//
// Created by 김 범주 on 30/09/2019.
//

#include <SquareTextureMaterial.hpp>

SquareTextureMaterial::SquareTextureMaterial(GLuint texUnit) : GLEngine::Material() {
    m_program->loadShaderFromFile("../../Resources/shader/BasicHolo/SquareTextureVertex.glsl", GL_VERTEX_SHADER);
    m_program->loadShaderFromFile("../../Resources/shader/BasicHolo/SquareTextureFragment.glsl", GL_FRAGMENT_SHADER);
    m_program->linkShader();

    glUseProgram(m_program->getProgramId());
    glUniform1i(glGetUniformLocation(m_program->getProgramId(), "tex"), texUnit);
}

