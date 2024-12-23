//
// Created by pmrj on 23-12-2024.
//

#pragma once

#include "mve/mve_window.h"

namespace mve {
    class FirstApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();

    private:
        MveWindow mveWindow {WIDTH, HEIGHT, "HELLO VULKAN!"};
    };
}




