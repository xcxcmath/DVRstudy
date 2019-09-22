//
// Created by bjk on 19. 9. 21..
//

#ifndef DVRSTUDY_AUTOGENAPP_HPP
#define DVRSTUDY_AUTOGENAPP_HPP

#include "DVRstudyAppBase.hpp"

class AutoGenApp : public DVRstudyAppBase {
protected:
    int m_frame_width;
    int m_frame_height;
    GLFWwindow* m_window;

    std::shared_ptr<GLEngine::Camera> m_camera;
    std::shared_ptr<GLEngine::Mesh> m_cubeMesh;

    static void createCube(const std::shared_ptr<GLEngine::Mesh>& mesh);

    void initWindow() override;
    void initGL() override;
    void initScene() override;
    void mainLoop() override;
    void cleanup() override;
public:
    explicit AutoGenApp(int frame_width, int frame_height);
};

#endif //DVRSTUDY_AUTOGENAPP_HPP
