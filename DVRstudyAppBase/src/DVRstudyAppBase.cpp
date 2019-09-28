//
// Created by bjk on 19. 9. 21..
//

#include "../include/DVRstudyAppBase.hpp"

DVRstudyAppBase::DVRstudyAppBase() = default;

DVRstudyAppBase::~DVRstudyAppBase() = default;

void DVRstudyAppBase::run() {
    initWindow();
    initGL();
    initScene();
    mainLoop();
    cleanup();
}
