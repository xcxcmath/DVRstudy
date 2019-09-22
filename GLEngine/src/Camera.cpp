//
// Created by bjk on 19. 8. 2..
//

#include <GLEngine/Camera.hpp>

GLEngine::Camera::Camera()
: m_transform(std::make_shared<GLEngine::Transform>())
{
    m_projection = glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
}

GLEngine::Camera::Camera(const GLEngine::Transform &transform, const glm::mat4& projection)
: m_transform(std::make_shared<GLEngine::Transform>(transform)), m_projection(projection)
{

}

GLEngine::Camera::Camera(const GLEngine::Camera &camera) {
    m_transform = std::make_shared<GLEngine::Transform>(*(camera.m_transform));
    m_projection = camera.m_projection;
}

GLEngine::Camera::Camera(GLEngine::Camera &&camera) noexcept {
    m_transform = std::move(camera.m_transform);
    m_projection = camera.m_projection;
}

const std::shared_ptr<GLEngine::Transform> &GLEngine::Camera::getTransform() const noexcept {
    return m_transform;
}

void GLEngine::Camera::setTransform(const GLEngine::Transform &transform) {
    *m_transform = transform;
}

void GLEngine::Camera::setTransform(GLEngine::Transform &&transform) {
    *m_transform = std::move(transform);
}

glm::mat4 GLEngine::Camera::getProjection() const noexcept {
    return m_projection;
}

void GLEngine::Camera::setProjection(const glm::mat4 &projection) {
    m_projection = projection;
}

GLEngine::Camera::~Camera() = default;
