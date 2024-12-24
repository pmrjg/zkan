//
// Created by pmrj on 23-12-2024.
//

#pragma once

#include "mve/mve_Pipeline.h"
#include "mve/mve_device.h"
#include "mve/mve_window.h"
#include "mve/mve_swap_chain.h"

#include <memory>
#include <vector>

namespace mve {
    class FirstApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp&) = delete;
        FirstApp& operator=(const FirstApp&) = delete;

        void run();

    private:
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();


        MveWindow mveWindow {WIDTH, HEIGHT, "HELLO VULKAN!"};
        MveDevice mveDevice {mveWindow};
        MveSwapChain mveSwapChain {mveDevice, mveWindow.getExtent()};
        std::unique_ptr<MvePipeline> mvePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
    };
}




