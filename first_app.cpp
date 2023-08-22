#include "first_app.hpp"

#include <stdexcept>
#include <array>

namespace game {

    FirstApp::FirstApp() {
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    FirstApp::~FirstApp() {
        vkDestroyPipelineLayout(gameDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run() {
        while (!gameWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(gameDevice.device());
    }

    void FirstApp::loadModels() {
        std::vector<GameModel::Vertex> vertiices {
            {{0, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}}
        };

        gameModel = std::make_unique<GameModel>(gameDevice, vertiices);
    }

    void FirstApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo vkPipelineLayoutInfo{};
        vkPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        vkPipelineLayoutInfo.setLayoutCount = 0;
        vkPipelineLayoutInfo.pSetLayouts = nullptr;
        vkPipelineLayoutInfo.pushConstantRangeCount = 0;
        vkPipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(gameDevice.device(), &vkPipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout");
        }
    }

    void FirstApp::createPipeline() {
        auto pipelineConfig = GamePipeline::defaultPipelineConfigInfo(gameSwapChain.width(), gameSwapChain.height());
        pipelineConfig.renderPass = gameSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        gamePipeline = std::make_unique<GamePipeline>(
            gameDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig
        );
    }
    
    void FirstApp::createCommandBuffers() {
        commandBuffers.resize(gameSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = gameDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(gameDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to create allocate buffers");
        }

        for (int i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            
            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("command buffer failed to begin recording");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = gameSwapChain.getRenderPass();
            renderPassInfo.framebuffer = gameSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = gameSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            gamePipeline->bind(commandBuffers[i]);
            gameModel->bind(commandBuffers[i]);
            gameModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);

            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer");
            }
        }
    }
    void FirstApp::drawFrame() {
        uint32_t imageIndex;
        auto result = gameSwapChain.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire next swapchain image");
        }

        result = gameSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swapchain image");
        }
    }
}