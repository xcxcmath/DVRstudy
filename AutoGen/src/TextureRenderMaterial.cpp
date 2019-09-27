//
// Created by 김 범주 on 22/09/2019.
//

#include <TextureRenderMaterial.hpp>

TextureRenderMaterial::TextureRenderMaterial(GLuint unit, int width, int height) : GLEngine::Material(), m_texUnit(unit){
    m_program->loadShaderFromFile("../../Resources/shader/AutoGenVertex.glsl", GL_VERTEX_SHADER);
    m_program->loadShaderFromFile("../../Resources/shader/AutoGenTexFragment.glsl", GL_FRAGMENT_SHADER);
    m_program->linkShader();

    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glGenTextures(1, &m_texID);
    glActiveTexture(GL_TEXTURE0 + m_texUnit);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texID, 0);

    glGenRenderbuffers(1, &m_buf);
    glBindRenderbuffer(GL_RENDERBUFFER, m_buf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, m_buf);

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw GLEngine::Exception("Framebuffer is not complete");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

TextureRenderMaterial::~TextureRenderMaterial() {
    glDeleteTextures(1, &m_texID);
    glDeleteRenderbuffers(1, &m_buf);
    glDeleteFramebuffers(1, &m_FBO);
}

GLuint TextureRenderMaterial::getFBO() const noexcept {
    return m_FBO;
}

