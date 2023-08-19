#pragma once

#include "game_device.hpp"

#include <string>
#include <vector>

namespace game {

    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class GamePipeline {
        public:
        GamePipeline(GameDevice &device, std::string vertFilePath, std::string fragFilePath, const PipelineConfigInfo& configInfo);

        ~GamePipeline();

        GamePipeline(const GamePipeline&) = delete;
        void operator=(const GamePipeline&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        
        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t heihgt);

        private:
        static std::vector<char> readFile(const std::string& filePath);

        void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        GameDevice& gameDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}