//
// Created by bjk on 19. 9. 21..
//

#ifndef DVRSTUDY_DVRSTUDYAPPBASE_HPP
#define DVRSTUDY_DVRSTUDYAPPBASE_HPP

#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <GLEngine/Camera.hpp>
#include <GLEngine/RenderObject.hpp>

class DVRstudyAppBase {
protected:
    virtual void initWindow() = 0;
    virtual void initGL() = 0;
    virtual void initScene() = 0;
    virtual void mainLoop() = 0;
    virtual void cleanup() = 0;

public:
    explicit DVRstudyAppBase();
    virtual ~DVRstudyAppBase();

    explicit DVRstudyAppBase(const DVRstudyAppBase&) = delete;
    explicit DVRstudyAppBase(DVRstudyAppBase&&) = delete;
    DVRstudyAppBase& operator=(const DVRstudyAppBase&) = delete;
    DVRstudyAppBase& operator=(DVRstudyAppBase&&) = delete;

    virtual void run();
};

#endif //DVRSTUDY_DVRSTUDYAPPBASE_HPP
