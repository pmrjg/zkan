//
// Created by pmrj on 28-12-2024.
//

#pragma once

namespace vkeng {
    struct GlfwInitialization {
    public:
        GlfwInitialization();
        ~GlfwInitialization();

        GlfwInitialization(const GlfwInitialization&) = delete;
        GlfwInitialization& operator=(const GlfwInitialization&) = delete;
    };
}
