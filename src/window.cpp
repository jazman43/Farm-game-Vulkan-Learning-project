#include "Headers/window.h"




Window::Window()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


    window = glfwCreateWindow(windowWidth, windowHeight, title, nullptr, nullptr);
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::CreateWindowSerface(VkInstance instance, VkSurfaceKHR *surface)
{
    //create surface

    if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
    {
        std::cerr << "Failed to create window surface\n";
    }else
    {
        std::cout << "window surface Created!\n";
    }

}

void Window::SetWindowSize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

int Window::GetWindowHeight()
{
    return windowHeight;
}

int Window::GetWindowWidth()
{
    return windowWidth;
}

GLFWwindow *Window::GetWindow()
{
    return window;
}