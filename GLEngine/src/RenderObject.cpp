//
// Created by bjk on 19. 8. 3..
//

#include <GLEngine/RenderObject.hpp>

GLEngine::RenderObject::RenderObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
: m_transform(std::make_shared<GLEngine::Transform>())
, m_mesh(std::move(mesh))
, m_material(std::move(material))
{

}

void GLEngine::RenderObject::render(const GLEngine::Camera &cam) const {
    auto programId = m_material->getProgram()->getProgramId();
    glUseProgram(programId);

    // send camera uniform data to glsl program
    // TODO : too strict condition to glsl?
    auto worldTransform = m_transform->getWorldTransform();
    auto cameraTransform = cam.getTransform()->getWorldTransform();
    auto projectionMatrix = cam.getProjection();

    glUniformMatrix4fv(glGetUniformLocation(programId, "worldTransform"), 1, GL_FALSE,
                       reinterpret_cast<float*>(&worldTransform));
    glUniformMatrix4fv(glGetUniformLocation(programId, "cameraTransform"), 1, GL_FALSE,
                       reinterpret_cast<float*>(&cameraTransform));
    glUniformMatrix4fv(glGetUniformLocation(programId, "projectionMatrix"), 1, GL_FALSE,
                       reinterpret_cast<float*>(&projectionMatrix));

    auto drawMode = m_mesh->getDrawMode();
    auto numElements = m_mesh->getNumElements();

    if(drawMode == GL_FALSE) {
        throw GLEngine::Exception("Cannot render this object with undefined behavior");
    }

    if(numElements == 0) {
        throw GLEngine::Exception("Cannot render this object with zero elements");
    }

    glBindVertexArray(m_mesh->getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, m_mesh->getVBO());

    if(m_mesh->hasIndex()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh->getIBO());
        glDrawElements(drawMode, numElements, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(drawMode, 0, numElements);
    }

    glBindVertexArray(0);

}

const std::shared_ptr<GLEngine::Transform> &GLEngine::RenderObject::getTransform() const noexcept {
    return m_transform;
}

void GLEngine::RenderObject::setMaterial(const std::shared_ptr<Material> &material) {
    m_material = material;
}

