//
// Created by bjk on 19. 8. 3..
//

#include <GLEngine/Base/Material.hpp>

GLEngine::Material::Material()
: m_program(std::make_shared<GLEngine::Program>())
{

}

const std::shared_ptr<GLEngine::Program> &GLEngine::Material::getProgram() const noexcept {
    return m_program;
}

GLEngine::Material::~Material() = default;
