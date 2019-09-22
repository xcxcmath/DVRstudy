//
// Created by bjk on 19. 8. 2..
//

#ifndef OPENGL_ENGINE_SHADER_HPP
#define OPENGL_ENGINE_SHADER_HPP

#include <string>

#include <GL/glew.h>
#include <GLEngine/Base/Exception.hpp>

namespace GLEngine {
    class Shader {
    protected:
        GLenum m_shaderType;
        std::string m_source;
        GLuint m_id;

    public:
        explicit Shader(std::string source, GLenum shaderType);
        Shader(const Shader&) = delete;
        Shader(Shader&&) = delete;
        virtual ~Shader();

        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) = delete;
        [[nodiscard]] GLuint getId() const noexcept;
    };
}

#endif //OPENGL_ENGINE_SHADER_HPP
