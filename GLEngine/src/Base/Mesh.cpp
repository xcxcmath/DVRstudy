//
// Created by bjk on 19. 8. 3..
//

#include <numeric>

#include <GLEngine/Base/Mesh.hpp>

GLEngine::Mesh::Mesh()
: m_vao(), m_vbo(), m_ibo()
, m_drawMode(GL_TRIANGLES)
, m_attributeSizes(), m_numElements(0)
, m_hasIndex(false), m_created(false)
, m_vertexData(), m_indexData()
{

}

GLEngine::Mesh::Mesh(const GLEngine::Mesh &mesh)
: m_vao(), m_vbo(), m_ibo()
, m_drawMode(mesh.m_drawMode)
, m_attributeSizes(mesh.m_attributeSizes), m_numElements(mesh.m_numElements)
, m_hasIndex(mesh.m_hasIndex), m_created(mesh.m_created)
, m_vertexData(mesh.m_vertexData), m_indexData(mesh.m_indexData)
{
    if(mesh.m_created) {
        createMesh();
    }
}

GLEngine::Mesh::Mesh(GLEngine::Mesh &&mesh) noexcept
: m_vao(mesh.m_vao), m_vbo(mesh.m_vbo), m_ibo(mesh.m_ibo)
, m_drawMode(mesh.m_drawMode)
, m_attributeSizes(std::move(mesh.m_attributeSizes)), m_numElements(mesh.m_numElements)
, m_hasIndex(mesh.m_hasIndex), m_created(mesh.m_created)
, m_vertexData(std::move(mesh.m_vertexData)), m_indexData(std::move(mesh.m_indexData))
{
    mesh.m_created = false;
}

GLEngine::Mesh::~Mesh() {
    if(m_created) {
        glDeleteBuffers(1, &m_vbo);
        if(m_hasIndex) {
            glDeleteBuffers(1, &m_ibo);
        }

        glBindVertexArray(m_vao);
        for(auto size : m_attributeSizes) {
            glDisableVertexAttribArray(size);
        }

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &m_vao);
    }
}

void GLEngine::Mesh::createMesh() {
    if(m_created) return;

    glGenBuffers(1, &m_vbo);
    setVBO(m_vertexData.data(), sizeof(float) * m_vertexData.size());

    if(m_hasIndex) {
        glGenBuffers(1, &m_ibo);
        setIBO(m_indexData.data(), sizeof(unsigned) * m_indexData.size());
    }

    m_created = true;
}

void GLEngine::Mesh::addVertexData(float data) {
    m_vertexData.emplace_back(data);
}

void GLEngine::Mesh::addVertexData(glm::vec2 data) {
    m_vertexData.insert(m_vertexData.cend(), {data.x, data.y});
}

void GLEngine::Mesh::addVertexData(glm::vec3 data) {
    m_vertexData.insert(m_vertexData.cend(), {data.x, data.y, data.z});
}

void GLEngine::Mesh::addVertexData(glm::vec4 data) {
    m_vertexData.insert(m_vertexData.cend(), {data.x, data.y, data.z, data.w});
}

void GLEngine::Mesh::addIndexData(unsigned data) {
    m_indexData.emplace_back(data);
}

void GLEngine::Mesh::addAttribute(GLuint data) {
    m_attributeSizes.emplace_back(data);
}

GLuint GLEngine::Mesh::getVAO() const noexcept {
    return m_vao;
}

GLuint GLEngine::Mesh::getVBO() const noexcept {
    return m_vbo;
}

GLuint GLEngine::Mesh::getIBO() const noexcept {
    return m_ibo;
}

void GLEngine::Mesh::setVBO(float *vertexData, size_t bufSize) {
    if(m_attributeSizes.empty()) {
        throw GLEngine::Exception("Attributes must be set to generate VBO");
    }

    // copy host data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, bufSize, vertexData, GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_vao);

    // set VAO
    GLsizei vertexSize = sizeof(float)
            * std::accumulate(m_attributeSizes.begin(), m_attributeSizes.end(), 0U);

    glBindVertexArray(m_vao);
    GLsizei currentOffset = 0U;
    for(size_t i = 0; i < m_attributeSizes.size(); ++i) {
        glVertexAttribPointer(i, m_attributeSizes[i], GL_FLOAT, GL_FALSE, vertexSize,
                reinterpret_cast<void*>(sizeof(float) * currentOffset));
        glEnableVertexAttribArray(i);
        currentOffset += m_attributeSizes[i];
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void GLEngine::Mesh::setIBO(unsigned *indexData, size_t bufSize) {
    if(!m_hasIndex) {
        throw GLEngine::Exception("This mesh is not index mode");
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufSize, indexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLenum GLEngine::Mesh::getDrawMode() const noexcept {
    return m_drawMode;
}

void GLEngine::Mesh::setDrawMode(GLenum drawMode) noexcept {
    m_drawMode = drawMode;
}

GLsizei GLEngine::Mesh::getNumElements() const noexcept {
    return m_numElements;
}

void GLEngine::Mesh::setNumElements(GLsizei numElements) noexcept {
    m_numElements = numElements;
}

bool GLEngine::Mesh::hasIndex() const noexcept {
    return m_hasIndex;
}

void GLEngine::Mesh::setHasIndex(bool hasIndex) noexcept {
    m_hasIndex = hasIndex;
}
