//
// Created by bjk on 19. 8. 2..
//

#include <algorithm>
#include <sstream>
#include <fstream>

#include <GLEngine/Base/Program.hpp>

GLEngine::Program::Program()
: m_shaders(), m_linked(false)
{
    m_programId = glCreateProgram();
    if(m_programId == 0) {
        throw GLEngine::Exception("GL could not create a program", glGetError());
    }
}

GLEngine::Program::~Program() {
    if(m_programId) {
        std::for_each(m_shaders.rbegin(), m_shaders.rend(), [this](std::unique_ptr<Shader> &ptr) {
            glDetachShader(m_programId, ptr->getId());
            ptr.reset(nullptr);
        });

        glDeleteProgram(m_programId);
    }
}

bool GLEngine::Program::attachShader(std::string source, GLenum shaderType) {
    if(m_linked) {
        return false;
    }

    // TODO : error check

    glUseProgram(m_programId);

    m_shaders.emplace_back(std::make_unique<Shader>(std::move(source), shaderType));
    glAttachShader(m_programId, m_shaders.back()->getId());

    return true;
}

bool GLEngine::Program::loadShaderFromFile(const std::string &filename, GLenum shaderType) {
    if(m_linked) {
        return false;
    }

    // TODO : error check

    std::ifstream shaderFile(filename);
    const bool FILE_OPENED = shaderFile.is_open();
    if(!FILE_OPENED){
        auto msg = std::string("GLSL file (") + filename + ") is not found";
        throw GLEngine::Exception(msg);
    }

    std::stringstream shaderContentBuffer;
    shaderContentBuffer << shaderFile.rdbuf();

    return attachShader(shaderContentBuffer.str(), shaderType);
}

void GLEngine::Program::linkShader() {
    if(m_linked) {
        return;
    }

    // TODO : exception VS just message

    glLinkProgram(m_programId);
    GLint error;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &error);
    if(error == GL_FALSE) {
        throw GLEngine::Exception("Failed to link program");
    }
    else m_linked = true;
}

GLuint GLEngine::Program::getProgramId() const noexcept {
    return m_programId;
}

