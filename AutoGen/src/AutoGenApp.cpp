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

    m_cubeMesh = std::make_shared<GLEngine::Mesh>();
    createCube(m_cubeMesh);
    //m_cubeMaterial = std::make_shared<VolumeTextureMaterial>(1, 0, "../../Resources/volume/Generated.raw", 128, 128, 128);
    m_cubeMaterial = std::make_shared<VolumeTextureMaterial>(1, 0, "../Resources/volume/MRI-Head.256x256x256.raw", 256, 256, 256);
    //m_cubeMaterial = std::make_shared<VolumeTextureMaterial>(1, 0, "../Resources/volume/Bonsai1.512x512x182.raw", 512, 512, 182);
    m_cubeTexMaterial = std::make_shared<TextureRenderMaterial>(0, m_frame_width, m_frame_height);
    m_cube = std::make_shared<GLEngine::RenderObject>(m_cubeMesh, m_cubeMaterial);
    m_cube->getTransform()->setScale(glm::vec3(1.4f,1.2f, 1.f));
    //m_cube->getTransform()->setScale(glm::vec3(1.f,0.5f, 1.f));
    m_cube->getTransform()->setOrientation(
            glm::rotate(glm::mat4(1.f), glm::radians(15.f), glm::vec3(1.f, 0.f, 0.f)));
}

void AutoGenApp::mainLoop() {
    auto prev_time = static_cast<float>(glfwGetTime());
    while(!glfwWindowShouldClose(m_window)) {
        const auto total_time = static_cast<float>(glfwGetTime());
        const auto elapsed_time = total_time - prev_time;
        prev_time = total_time;

        float rotate = elapsed_time / 2;
        m_cube->getTransform()->setOrientation(glm::rotate(
                m_cube->getTransform()->getOrientation(),
                rotate,
                glm::vec3(0.f, 1.f, 0.f)));

        glBindFramebuffer(GL_FRAMEBUFFER, m_cubeTexMaterial->getFBO());
        glClearColor((GLclampf) 0.f, (GLclampf) 0.f, (GLclampf) 0.f, (GLclampf) 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        m_cube->setMaterial(m_cubeTexMaterial);
        m_cube->render(*m_camera);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor((GLclampf) 0.f, (GLclampf) 0.f, (GLclampf) 0.f, (GLclampf) 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);
        m_cube->setMaterial(m_cubeMaterial);
        m_cubeMaterial->updateTime(total_time);
        m_cubeMaterial->updateSigma(0.98f + 0.003f * glm::sin(total_time));
        //m_cubeMaterial->updateSigma(19.0378f);
        m_cube->render(*m_camera);

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

