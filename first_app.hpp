#pragma once

#include "game_window.hpp"
#include "game_pipeline.hpp"
#include "game_swap_chain.hpp"
#include "game_device.hpp"
#include "game_model.hpp"

#include <memory>
#include <vector>

namespace game
{
    class FirstApp
    {

    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        FirstApp();
        ~FirstApp();

        FirstApp(const GameWindow &) = delete;
        FirstApp &operator=(const GameWindow &) = delete;
            
        void run();

    private:
        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        GameWindow gameWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
        GameDevice gameDevice{gameWindow};
        GameSwapChain gameSwapChain{gameDevice, gameWindow.getExtent()};
        std::unique_ptr<GamePipeline> gamePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<GameModel> gameModel;
    };
}