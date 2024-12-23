//
// Created by pmrj on 23-12-2024.
//

#pragma once

#include "mve/mve_Pipeline.h"
#include "mve/mve_device.h"
#include "mve/mve_window.h"

namespace mve {
    class FirstApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        FirstApp(){};
        ~FirstApp(){};

        FirstApp(const FirstApp&) = delete;
        FirstApp& operator=(const FirstApp&) = delete;

        void run();

    private:
        MveWindow mveWindow {WIDTH, HEIGHT, "HELLO VULKAN!"};
        MveDevice mveDevice {mveWindow};
        MvePipeline mvePipeline{mveDevice, "./shaders/simple_shader.vert.spv", "./shaders/simple_shader.frag.spv", MvePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}




