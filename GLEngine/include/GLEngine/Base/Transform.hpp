//
// Created by bjk on 19. 8. 2..
//

#ifndef OPENGL_ENGINE_TRANSFORM_HPP
#define OPENGL_ENGINE_TRANSFORM_HPP

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GLEngine {
    class Transform {
    protected:
        glm::vec3 m_position;
        glm::mat4 m_orientation;
        glm::vec3 m_scale;

        std::shared_ptr<Transform> m_parent;

    public:
        static glm::vec3 interpolatePosition(const Transform &zero, const Transform &one, float t);
        static glm::mat4 interpolateOrientation(const Transform &zero, const Transform &one, float t);
        static glm::vec3 interpolateScale(const Transform &zero, const Transform &one, float t);
        static Transform interpolate(const Transform &zero, const Transform &one, float t);

        explicit Transform() noexcept;
        explicit Transform(const glm::vec3& position, const glm::mat4& orientation, const glm::vec3& scale) noexcept;
        Transform(const Transform& transform);
        Transform(Transform&& transform) noexcept;
        virtual ~Transform();

        Transform& operator=(const Transform& transform);
        Transform& operator=(Transform&& transform) noexcept;

        [[nodiscard]] glm::vec3 getPosition() const noexcept;
        void setPosition(const glm::vec3& position);

        [[nodiscard]] glm::mat4 getOrientation() const noexcept;
        void setOrientation(const glm::mat4& orientation);

        [[nodiscard]] glm::vec3 getScale() const noexcept;
        void setScale(const glm::vec3& scale);

        [[nodiscard]] const std::shared_ptr<GLEngine::Transform>& getParent() const noexcept;
        void setParent(const std::shared_ptr<Transform>& parent);
        void setParent(std::shared_ptr<Transform>&& parent);

        [[nodiscard]] glm::mat4 getWorldTransform() const;
    };
}

#endif //OPENGL_ENGINE_TRANSFORM_HPP
