//
// Created by pmrj on 23-12-2024.
//

#include "mve_Pipeline.h"

#include <iostream>
#include <fstream>

namespace mve {
     MvePipeline::MvePipeline(MveDevice &device, const std::string &vertFilePath, const std::string &fragFilepath,
                             const PipelineConfigInfo &configInfo) : mveDevice{device} {
         createGraphicsPipeline(vertFilePath, fragFilepath, configInfo);
     }

     MvePipeline::~MvePipeline() { }


    PipelineConfigInfo MvePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
         PipelineConfigInfo configInfo{};

         configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
         configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
         configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

         configInfo.viewport.x = 0.0f;
         configInfo.viewport.y = 0.0f;
         configInfo.viewport.width = static_cast<float>(width);
         configInfo.viewport.height = static_cast<float>(height);
         configInfo.viewport.minDepth = 0.0f;
         configInfo.viewport.maxDepth = 1.0f;

         configInfo.scissor.offset = {0, 0};
         configInfo.scissor.extent = {width, height};

         configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
         configInfo.viewportInfo.viewportCount = 1;
         configInfo.viewportInfo.pViewports = &configInfo.viewport;
         configInfo.viewportInfo.scissorCount = 1;
         configInfo.viewportInfo.pScissors = &configInfo.scissor;

         configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
         configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
         configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
         configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
         configInfo.rasterizationInfo.lineWidth = 1.0f;
         configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
         configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
         configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
         configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
         configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
         configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

         return configInfo;
     }

     std::vector<char> MvePipeline::readFile(const std::string &filename) {
         std::ifstream file(filename, std::ios::ate | std::ios::binary);

         if (!file.is_open()) {
             throw std::runtime_error("Failed to open file " + filename);
         }

         size_t fileSize = static_cast<size_t>(file.tellg());

         std::vector<char> buffer(fileSize);
         file.seekg(0);
         file.read(buffer.data(), fileSize);
         file.close();
         return buffer;
     }

    void MvePipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo) {
         auto vertCode = readFile(vertFilepath);
         auto fragCode = readFile(fragFilepath);

         std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;
         std::cout << "Fragment Shader Code Size: " << fragCode.size() << std::endl;
     }

     void MvePipeline::createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule) {
         VkShaderModuleCreateInfo createInfo{};
         createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
         createInfo.codeSize = code.size();
         createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

         if (vkCreateShaderModule(mveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
             throw std::runtime_error("failed to create shader module");
         }
     }


} // namespace mve
