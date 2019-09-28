//
// Created by bjk on 19. 9. 29..
//

#include <BasicHoloApp.hpp>

void BasicHoloApp::createCube(const std::shared_ptr<Mesh> &mesh) {
    mesh->addAttribute(3);
    mesh->setHasIndex(true);

    std::vector<glm::vec3> pos = {
            glm::vec3(-1.f,-1.f,1.f),
            glm::vec3(1.f,-1.f,1.f),
            glm::vec3(1.f,1.f,1.f),
            glm::vec3(-1.f,1.f,1.f),
            glm::vec3(-1.f,-1.f,-1.f),
            glm::vec3(1.f,-1.f,-1.f),
            glm::vec3(1.f,1.f,-1.f),
            glm::vec3(-1.f,1.f,-1.f)
    };
    std::vector<unsigned> indices = {
            0,1,3,1,2,3,
            0,3,4,4,3,7,
            1,5,2,5,6,2,
            3,6,7,3,2,6,
            0,4,5,0,5,1,
            4,7,6,4,6,5
    };

    for(auto po: pos) mesh->addVertexData(po);
    for(auto i: indices) mesh->addIndexData(i);

    mesh->setNumElements(indices.size());
    mesh->createMesh();
}

void BasicHoloApp::createSquare(const std::shared_ptr<Mesh> &mesh) {
    mesh->addAttribute(2);
    mesh->setDrawMode(GL_TRIANGLE_FAN);

    std::vector<glm::vec2> pos = {
            glm::vec2(-1.f, 1.f),
            glm::vec2(-1.f, -1.f),
            glm::vec2(1.f, -1.f),
            glm::vec2(1.f, 1.f)
    };

    for(auto po: pos) mesh->addVertexData(po);
    mesh->setNumElements(pos.size());
    mesh->createMesh();
}

void BasicHoloApp::initWindow() {

}

void BasicHoloApp::initGL() {

}

void BasicHoloApp::initScene() {

}

void BasicHoloApp::mainLoop() {

}

void BasicHoloApp::cleanup() {

}

BasicHoloApp::BasicHoloApp(int frame_width, int frame_height)
: DVRstudyAppBase()
, m_frame_width(frame_width)
, m_frame_height(frame_height)
, m_window(nullptr) {

}
