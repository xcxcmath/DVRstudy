//
// Created by bjk on 19. 8. 2..
//

#ifndef OPENGL_ENGINE_EXCEPTION_HPP
#define OPENGL_ENGINE_EXCEPTION_HPP

#include <string>
#include <exception>
#include <GL/glew.h>

namespace GLEngine {
class Exception : public std::exception {
    protected:
        std::string m_message;
    public:
        explicit Exception(const std::string& errorString, GLenum errorCode);
        explicit Exception(std::string errorString);
        ~Exception() override;

        [[nodiscard]] const char* what() const noexcept override;
        [[nodiscard]] virtual std::string str() const noexcept;
    };
}

#endif //OPENGL_ENGINE_EXCEPTION_HPP
