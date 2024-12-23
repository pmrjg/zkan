//
// Created by pmrj on 23-12-2024.
//

#include "first_app.h"

namespace mve {
    void FirstApp::run() {
        while (!mveWindow.shouldClose()) {
            glfwPollEvents();
            mveWindow.update();
        }
    }
}