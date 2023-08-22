#pragma once

#include "game_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace game {
    class GameModel {
        public:

        struct Vertex {
            glm::vec2 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingsDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributesDescriptions();
        };

        GameModel(GameDevice &device, const std::vector<Vertex> &verticies);
        ~GameModel();

        GameModel(const GameWindow &) = delete;
        GameModel &operator=(const GameWindow &) = delete;
        
        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        private:
            void createVertexBuffers(const std::vector<Vertex> &verticies);

            GameDevice& gameDevice;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            uint32_t vertextCount;
    };
}