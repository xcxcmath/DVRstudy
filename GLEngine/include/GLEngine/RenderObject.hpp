//
// Created by bjk on 19. 8. 3..
//

#ifndef OPENGL_ENGINE_RENDEROBJECT_HPP
#define OPENGL_ENGINE_RENDEROBJECT_HPP

#include <GLEngine/Base/Mesh.hpp>
#include <GLEngine/Base/Material.hpp>
#include <GLEngine/Camera.hpp>

namespace GLEngine {
    class RenderObject {
    protected:
        std::shared_ptr<Transform> m_transform;
        std::shared_ptr<Mesh> m_mesh;
        std::shared_ptr<Material> m_material;

    public:
        explicit RenderObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
        void render(const Camera& cam) const;

        // TODO : implement below
        /*
        [[nodiscard]] const std::shared_ptr<Mesh> &getMesh() const noexcept;
        void setMesh(const std::shared_ptr<Mesh> &mesh);
        void setMesh(std::shared_ptr<Mesh> &&mesh) noexcept;

        [[nodiscard]] const std::shared_ptr<Material> &getMaterial() const noexcept;
        void setMaterial(const std::shared_ptr<Material> &material);
        void setMaterial(std::shared_ptr<Material> &&material);
         */
        void setMaterial(const std::shared_ptr<Material> &material);

        [[nodiscard]] const std::shared_ptr<Transform> &getTransform() const noexcept;
    };
}

#endif //OPENGL_ENGINE_RENDEROBJECT_HPP
