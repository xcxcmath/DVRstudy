//
// Created by bjk on 19. 8. 3..
//

#ifndef OPENGL_ENGINE_MATERIAL_HPP
#define OPENGL_ENGINE_MATERIAL_HPP

#include <glm/glm.hpp>
#include <GLEngine/Base/Program.hpp>

namespace GLEngine {
    class Material {
    protected:
        std::shared_ptr<Program> m_program;

    public:
        explicit Material();
        virtual ~Material();

        [[nodiscard]] const std::shared_ptr<Program>& getProgram() const noexcept;
    };
}

#endif //OPENGL_ENGINE_MATERIAL_HPP
