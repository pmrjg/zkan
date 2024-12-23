//
// Created by pmrj on 23-12-2024.
//

#pragma once

#include "mve_device.h"

#include <string>
#include <vector>

namespace mve {
    struct PipelineConfigInfo {
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class MvePipeline {
    public:
        MvePipeline(MveDevice&, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
        ~MvePipeline();

        MvePipeline(const MvePipeline&) = delete;
        MvePipeline& operator=(const MvePipeline&) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
    private:
        static std::vector<char> readFile(const std::string& filename);

        void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        MveDevice& mveDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}



