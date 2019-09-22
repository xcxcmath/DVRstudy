//
// Created by bjk on 19. 8. 2..
//

#include <GLEngine/Base/Transform.hpp>

glm::vec3 GLEngine::Transform::interpolatePosition(const Transform &zero, const Transform &one, float t) {
    return glm::mix(zero.m_position, one.m_position, t);
}

glm::mat4
GLEngine::Transform::interpolateOrientation(const GLEngine::Transform &zero, const GLEngine::Transform &one, float t) {
    return glm::mat4_cast(
            glm::slerp(
                    glm::quat_cast(zero.m_orientation),
                    glm::quat_cast(one.m_orientation),
                    t
                    ));
}

glm::vec3
GLEngine::Transform::interpolateScale(const GLEngine::Transform &zero, const GLEngine::Transform &one, float t) {
    return glm::mix(zero.m_scale, one.m_scale, t);
}

GLEngine::Transform
GLEngine::Transform::interpolate(const GLEngine::Transform &zero, const GLEngine::Transform &one, float t) {
    return Transform(interpolatePosition(zero, one, t),
            interpolateOrientation(zero, one, t),
            interpolateScale(zero, one, t));
}

GLEngine::Transform::Transform() noexcept
: m_position(0.f), m_orientation(1.f), m_scale(1.f), m_parent(nullptr)
{

}

GLEngine::Transform::Transform(const glm::vec3 &position, const glm::mat4 &orientation,
                               const glm::vec3 &scale) noexcept
: m_position(position), m_orientation(orientation), m_scale(scale), m_parent(nullptr)
{

}

GLEngine::Transform::Transform(const GLEngine::Transform &transform)
: m_position(transform.m_position), m_orientation(transform.m_orientation), m_scale(transform.m_scale)
, m_parent(transform.m_parent)
{

}

GLEngine::Transform::Transform(GLEngine::Transform &&transform) noexcept
        : m_position(transform.m_position), m_orientation(transform.m_orientation), m_scale(transform.m_scale)
        , m_parent(std::move(transform.m_parent))
{

}

GLEngine::Transform &GLEngine::Transform::operator=(const GLEngine::Transform &transform) = default;

GLEngine::Transform &GLEngine::Transform::operator=(GLEngine::Transform &&transform) noexcept {
    m_position = transform.m_position;
    m_orientation = transform.m_orientation;
    m_scale = transform.m_scale;
    m_parent = std::move(transform.m_parent);
    return *this;
}

glm::vec3 GLEngine::Transform::getPosition() const noexcept {
    return m_position;
}

void GLEngine::Transform::setPosition(const glm::vec3 &position) {
    m_position = position;
}

glm::mat4 GLEngine::Transform::getOrientation() const noexcept {
    return m_orientation;
}

void GLEngine::Transform::setOrientation(const glm::mat4 &orientation) {
    m_orientation = orientation;
}

glm::vec3 GLEngine::Transform::getScale() const noexcept {
    return m_scale;
}

void GLEngine::Transform::setScale(const glm::vec3 &scale) {
    m_scale = scale;
}

const std::shared_ptr<GLEngine::Transform> &GLEngine::Transform::getParent() const noexcept {
    return m_parent;
}

void GLEngine::Transform::setParent(const std::shared_ptr<Transform> &parent) {
    m_parent = parent;
}

void GLEngine::Transform::setParent(std::shared_ptr<Transform> &&parent) {
    m_parent = std::move(parent);
}

glm::mat4 GLEngine::Transform::getWorldTransform() const {
    auto ret = glm::translate(glm::mat4(1.f), m_position)
            * m_orientation
            * glm::scale(glm::mat4(1.f), m_scale);

    if(m_parent != nullptr) {
        ret = m_parent->getWorldTransform()
                * glm::scale(glm::mat4(1.f), m_parent->m_scale)
                * ret;
    }

    return ret;
}

GLEngine::Transform::~Transform() = default;

