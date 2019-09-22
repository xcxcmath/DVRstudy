//
// Created by bjk on 19. 8. 2..
//

#include <GLEngine/Base/Shader.hpp>

#include <algorithm>

GLEngine::Shader::Shader(std::string source, GLenum shaderType)
: m_shaderType(shaderType), m_source(std::move(source)), m_id(0)
{
    const auto supportTypes = {GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER};
    const bool SUPPORTED_TYPE = std::any_of(supportTypes.begin(), supportTypes.end(),
            [shaderType](const auto& type) {
        return type == shaderType;
    });

    if(!SUPPORTED_TYPE) {
        throw GLEngine::Exception("Shader type not supported");
    }

    if((m_id = glCreateShader(shaderType)) != 0) {
        //m_source = shaderContent.str();
        const auto sourceChar = m_source.c_str();
        glShaderSource(m_id, 1, &sourceChar, nullptr);
    } else {
        throw GLEngine::Exception("GL could not create new shader", glGetError());
    }

    glCompileShader(m_id);
    GLint status = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(m_id, infoLogLength, nullptr, infoLog);
        std::string infoLogStr(infoLog);
        delete[] infoLog;
        throw GLEngine::Exception(std::string("Shader could not compile: ") + infoLogStr);
    }
}

GLEngine::Shader::~Shader() {
    if(m_id) glDeleteShader(m_id);
}

GLuint GLEngine::Shader::getId() const noexcept {
    return m_id;
}
