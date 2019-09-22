//
// Created by bjk on 19. 8. 2..
//

#include <GLEngine/Base/Exception.hpp>
#include <sstream>

GLEngine::Exception::Exception(const std::string& errorString, GLenum errorCode)
{
    const auto codeString = gluGetString(errorCode);
    std::stringstream msg_stream;
    msg_stream << "[GLEngine Exception] " << errorString << " ";
    msg_stream << "code " << errorCode;
    msg_stream << " : " << codeString;

    m_message = msg_stream.str();
}

GLEngine::Exception::Exception(std::string errorString) {
    m_message = "[GLEngine Exception] ";
    m_message = m_message + std::move(errorString);
}

GLEngine::Exception::~Exception() = default;

const char *GLEngine::Exception::what() const noexcept {
    return m_message.c_str();
}

std::string GLEngine::Exception::str() const noexcept {
    return m_message;
}


