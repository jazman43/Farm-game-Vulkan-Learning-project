#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

class Window
{
public:

    Window();
    ~Window();

    void CreateWindowSerface(VkInstance instance, VkSurfaceKHR *surface);

    int GetWindowWidth();
    int GetWindowHeight();

    void SetWindowSize(int width, int height);

    bool ShouldClose() { return glfwWindowShouldClose(window); }

    VkExtent2D GetExtent() { return {static_cast<uint32_t>(windowWidth), static_cast<uint32_t>(windowHeight)}; }

    GLFWwindow *GetWindow();

private:
    int windowWidth = 1920;
    int windowHeight = 1080;

    const char* title = "Farm Game";

    GLFWwindow* window;

};