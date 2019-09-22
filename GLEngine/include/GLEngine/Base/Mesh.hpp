//
// Created by bjk on 19. 8. 3..
//

#ifndef OPENGL_ENGINE_MESH_HPP
#define OPENGL_ENGINE_MESH_HPP

#include <vector>

#include <GLEngine/Base/Exception.hpp>
#include <glm/glm.hpp>

namespace GLEngine {
    class Mesh {
    protected:
        GLuint m_vao, m_vbo, m_ibo;
        GLenum m_drawMode;
        std::vector<GLuint> m_attributeSizes;

        GLsizei m_numElements;
        bool m_hasIndex, m_created;

        std::vector<float> m_vertexData;
        std::vector<unsigned> m_indexData;

        void setVBO(float* vertexData, size_t bufSize);
        void setIBO(unsigned* indexData, size_t bufSize);

    public:
        explicit Mesh();
        Mesh(const Mesh& mesh);
        Mesh(Mesh&& mesh) noexcept;
        virtual ~Mesh();

        Mesh& operator=(const Mesh& mesh) = delete;
        Mesh& operator=(Mesh&& mesh) noexcept = delete;

        void createMesh();
        void addVertexData(float data);
        void addVertexData(glm::vec2 data);
        void addVertexData(glm::vec3 data);
        void addVertexData(glm::vec4 data);

        void addIndexData(unsigned data);
        void addAttribute(GLuint data);

        [[nodiscard]] GLuint getVAO() const noexcept;
        [[nodiscard]] GLuint getVBO() const noexcept;
        [[nodiscard]] GLuint getIBO() const noexcept;
        [[nodiscard]] GLenum getDrawMode() const noexcept;
        void setDrawMode(GLenum drawMode) noexcept;
        [[nodiscard]] GLsizei getNumElements() const noexcept;
        void setNumElements(GLsizei numElements) noexcept;
        [[nodiscard]] bool hasIndex() const noexcept;
        void setHasIndex(bool hasIndex) noexcept;

    };
}

#endif //OPENGL_ENGINE_MESH_HPP
