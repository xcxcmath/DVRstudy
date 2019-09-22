//
// Created by bjk on 19. 9. 21..
//

#include <AutoGenApp.hpp>

void AutoGenApp::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_frame_width, m_frame_height, "AutoGen", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
}

void AutoGenApp::initGL() {
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_3D);
}

void AutoGenApp::initScene() {
    m_camera = std::make_shared<GLEngine::Camera>();
    m_camera->getTransform()->setPosition(glm::vec3(0.f, 0.f, 2.5f));
    m_camera->setProjection(glm::perspective(glm::radians(90.f), 4.f/3.f, 0.01f, 30.f));

    createCube(m_cubeMesh);

}

void AutoGenApp::mainLoop() {
    auto prev_time = static_cast<float>(glfwGetTime());
    while(!glfwWindowShouldClose(m_window)) {
        const auto total_time = static_cast<float>(glfwGetTime());
        const auto elapsed_time = total_time - prev_time;
        prev_time = total_time;

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void AutoGenApp::cleanup() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

AutoGenApp::AutoGenApp(int frame_width, int frame_height)
        : DVRstudyAppBase(), m_frame_width(frame_width), m_frame_height(frame_height), m_window(nullptr),
        m_cubeMesh(nullptr)
{

}

void AutoGenApp::createCube(const std::shared_ptr<GLEngine::Mesh> &mesh) {
    mesh->addAttribute(3);
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

    for(auto po: pos) {
        mesh->addVertexData(po);
        mesh->addVertexData((po+glm::vec3(1.f))/2.f);
    }
    for(auto i: indices) mesh->addIndexData(i);

    mesh->setNumElements(indices.size());
    mesh->createMesh();
}

