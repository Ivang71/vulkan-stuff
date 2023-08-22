#include "game_model.hpp"

#include <cassert>
#include <cstring>

namespace game {
    GameModel::GameModel(GameDevice &device, const std::vector<Vertex> &verticies): gameDevice{device} {
        createVertexBuffers(verticies);
    }
    GameModel::~GameModel() {
        vkDestroyBuffer(gameDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(gameDevice.device(), vertexBufferMemory, nullptr);
    }
    void GameModel::createVertexBuffers(const std::vector<Vertex> &verticies) {
        vertextCount = static_cast<uint32_t>(verticies.size());
        assert(vertextCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(verticies[0]) * vertextCount;
        gameDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vertexBuffer,
            vertexBufferMemory
        );

        void* data;
        vkMapMemory(gameDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, verticies.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(gameDevice.device(), vertexBufferMemory);
    }

    void GameModel::draw(VkCommandBuffer commandBuffer) {
        vkCmdDraw(commandBuffer, vertextCount, 1, 0, 0);
    }

    void GameModel::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    std::vector<VkVertexInputBindingDescription> GameModel::Vertex::getBindingsDescriptions() {
        return {{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}};
    }

    std::vector<VkVertexInputAttributeDescription> GameModel::Vertex::getAttributesDescriptions() {
        return {{0, 0, VK_FORMAT_R32G32_SFLOAT, 0}};
    }
}