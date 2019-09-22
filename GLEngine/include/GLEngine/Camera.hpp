//
// Created by bjk on 19. 8. 2..
//

#ifndef OPENGL_ENGINE_CAMERA_HPP
#define OPENGL_ENGINE_CAMERA_HPP

#include <GLEngine/Base/Transform.hpp>

namespace GLEngine {
    class Camera {
    protected:
        std::shared_ptr<Transform> m_transform;
        glm::mat4 m_projection;

    public:
        explicit Camera();
        explicit Camera(const Transform& transform, const glm::mat4& projection);
        Camera(const Camera& camera);
        Camera(Camera&& camera) noexcept;
        virtual ~Camera();

        [[nodiscard]] const std::shared_ptr<Transform>& getTransform() const noexcept;
        void setTransform(const Transform& transform);
        void setTransform(Transform&& transform);

        [[nodiscard]] glm::mat4 getProjection() const noexcept;
        void setProjection(const glm::mat4& projection);

    };
}

#endif //OPENGL_ENGINE_CAMERA_HPP
