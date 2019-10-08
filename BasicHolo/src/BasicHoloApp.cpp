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
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_frame_width, m_frame_height, "BasicHolo", nullptr, nullptr);
    glfwGetFramebufferSize(m_window, &m_frame_width, &m_frame_height);
    glfwMakeContextCurrent(m_window);
}

void BasicHoloApp::initGL() {
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //glEnable(GL_TEXTURE_1D);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_3D);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void BasicHoloApp::initScene() {
    createRenderTexture(4, m_texRenderWidth, m_texRenderHeight);

    m_texRenderCamera = std::make_shared<GLEngine::Camera>();
    m_texRenderCamera->getTransform()->setPosition(glm::vec3(0.f, 0.f, 2.f));
    m_texRenderCamera->setProjection(glm::perspective(glm::radians(90.f),
            static_cast<float>(m_texRenderWidth) / m_texRenderHeight, 0.01f, 30.f));

    m_showCamera = std::make_shared<Camera>();
    auto show_ratio = static_cast<float>(m_frame_width) / m_frame_height;
    m_showCamera->setProjection(glm::ortho(-3.f * show_ratio, 3.f * show_ratio, -3.f, 3.f, -3.f, 3.f));

    m_cubeMesh = std::make_shared<GLEngine::Mesh>();
    createCube(m_cubeMesh);
    //m_cubeMaterial = std::make_shared<CubeTextureMaterial>(1, 0, "../../Resources/volume/Generated.raw", 128, 128, 128);
    m_cubeMaterial = std::make_shared<CubeTextureMaterial>(1, 0, "../../Resources/volume/MRI-Head.256x256x256.raw", 256, 256, 256);
    //m_cubeMaterial = std::make_shared<CubeTextureMaterial>(1, 0, "../../Resources/volume/Bonsai1.512x512x182.raw", 512, 512, 182);
    m_cubeTexMaterial = std::make_shared<TexCoordRenderMaterial>(0, m_texRenderWidth, m_texRenderHeight);
    m_cube = std::make_shared<GLEngine::RenderObject>(m_cubeMesh, m_cubeMaterial);
    m_cube->getTransform()->setScale(glm::vec3(1.4f,1.2f, 1.f));
    //m_cube->getTransform()->setScale(glm::vec3(1.f,0.5f, 1.f));
    m_cube->getTransform()->setOrientation(
            glm::rotate(glm::mat4(1.f), glm::radians(20.f), glm::vec3(1.f, 0.f, 0.f)));

    m_squareMesh = std::make_shared<Mesh>();
    createSquare(m_squareMesh);
    m_squareMaterial = std::make_shared<SquareTextureMaterial>(4);
    m_square = std::make_shared<RenderObject>(m_squareMesh, m_squareMaterial);
    m_square->getTransform()->setScale(glm::vec3(-1.f, 1.f, 1.f) * 0.8f);

}

void BasicHoloApp::mainLoop() {
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
        glViewport(0, 0, m_texRenderWidth, m_texRenderHeight);
        glClearColor((GLclampf) 0.f, (GLclampf) 0.f, (GLclampf) 0.f, (GLclampf) 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        m_cube->setMaterial(m_cubeTexMaterial);
        m_cube->render(*m_texRenderCamera);

        glBindFramebuffer(GL_FRAMEBUFFER, m_texRenderFBO);
        //glViewport(0, 0, m_texRenderWidth, m_texRenderHeight);
        glClearColor((GLclampf) 0.f, (GLclampf) 0.f, (GLclampf) 0.f, (GLclampf) 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);
        m_cube->setMaterial(m_cubeMaterial);
        m_cubeMaterial->updateTime(total_time);
        //m_cubeMaterial->updateSigma(40.f + 4.f * glm::sin(total_time));
        //m_cubeMaterial->updateSigma(50.f);
        m_cube->render(*m_texRenderCamera);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_frame_width, m_frame_height);
        glCullFace(GL_FRONT);
        glClearColor((GLclampf) 0.f, (GLclampf) 0.f, (GLclampf) 0.f, (GLclampf) 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(int i = 0; i < 4; ++i){
            const auto angle = glm::radians(90.f) * static_cast<float>(i);
            m_square->getTransform()->setPosition(glm::vec3(
                    -glm::sin(angle), glm::cos(angle), 0
                    ) * 1.5f);
            m_square->getTransform()->setOrientation(glm::rotate(
                    glm::mat4(1.f), angle, glm::vec3(0.f, 0.f, 1.f)
                    ));
            m_square->render(*m_showCamera);
        }

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void BasicHoloApp::cleanup() {
    clearRenderTexture();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

BasicHoloApp::BasicHoloApp(int frame_width, int frame_height, int tex_width, int tex_height)
: DVRstudyAppBase()
, m_frame_width(frame_width)
, m_frame_height(frame_height)
, m_texRenderWidth(tex_width)
, m_texRenderHeight(tex_height)
, m_window(nullptr)
, m_texRenderMade(false) {

}

void BasicHoloApp::createRenderTexture(GLuint unit, GLuint width, GLuint height) {
    if(m_texRenderMade) return;
    glGenFramebuffers(1, &m_texRenderFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_texRenderFBO);

    glGenTextures(1, &m_texRenderTexture);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_texRenderTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texRenderTexture, 0);

    glGenRenderbuffers(1, &m_texRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_texRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, m_texRenderBuffer);

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw GLEngine::Exception("Framebuffer is not complete");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_texRenderMade = true;
}

void BasicHoloApp::clearRenderTexture() {
    if(m_texRenderMade){
        glDeleteTextures(1, &m_texRenderTexture);
        glDeleteRenderbuffers(1, &m_texRenderBuffer);
        glDeleteFramebuffers(1, &m_texRenderFBO);
    }
}
