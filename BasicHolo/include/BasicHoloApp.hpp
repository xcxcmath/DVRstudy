//
// Created by bjk on 19. 9. 29..
//

#ifndef DVRSTUDY_BASICHOLOAPP_HPP
#define DVRSTUDY_BASICHOLOAPP_HPP

#include <DVRstudyAppBase.hpp>

class BasicHoloApp : public DVRstudyAppBase {
protected:
    using Camera = GLEngine::Camera;
    using Mesh = GLEngine::Mesh;
    using RenderObject = GLEngine::RenderObject;

    int m_frame_width;
    int m_frame_height;
    GLFWwindow* m_window;

    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Mesh> m_cubeMesh, m_squareMesh;
    //std::shared_ptr<VolumeTextureMaterial> m_cubeMaterial;
    //std::shared_ptr<TextureRenderMaterial> m_cubeTexMaterial;
    std::shared_ptr<RenderObject> m_cube, m_square;

    static void createCube(const std::shared_ptr<Mesh>& mesh);
    static void createSquare(const std::shared_ptr<Mesh>& mesh);

    void initWindow() override;
    void initGL() override;
    void initScene() override;
    void mainLoop() override;
    void cleanup() override;
public:
    explicit BasicHoloApp(int frame_width, int frame_height);
};

#endif //DVRSTUDY_BASICHOLOAPP_HPP
