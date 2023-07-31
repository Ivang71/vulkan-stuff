#include "game_window.hpp"

namespace game {

    GameWindow::GameWindow(int w, int h, std::string name) : width{w}, height{h}, name{name} {
        initWindow();
    }

    GameWindow::~GameWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void GameWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    }
}
