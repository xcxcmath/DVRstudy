//
// Created by bjk on 19. 8. 2..
//

#ifndef OPENGL_ENGINE_PROGRAM_HPP
#define OPENGL_ENGINE_PROGRAM_HPP

#include <vector>
#include <memory>

#include <GLEngine/Base/Shader.hpp>

namespace GLEngine {
    class Program {
    protected:
        std::vector<std::unique_ptr<Shader>> m_shaders;
        bool m_linked;
        GLuint m_programId;

    public:
        explicit Program();
        Program(const Program&) = delete;
        Program(Program&&) = delete;
        virtual ~Program();

        Program& operator=(const Program&) = delete;
        Program& operator=(Program&&) = delete;

        bool attachShader(std::string source, GLenum shaderType);
        bool loadShaderFromFile(const std::string& filename, GLenum shaderType);
        void linkShader();
        [[nodiscard]] GLuint getProgramId() const noexcept;
    };
}

#endif //OPENGL_ENGINE_PROGRAM_HPP
