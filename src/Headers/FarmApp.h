#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>


#include <iostream>
#include <vector>
#include <optional>
#include <set>
#include <sstream>
#include <fstream>


struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};


class FarmApp
{
public:
    void Run();

private:
    GLFWwindow* window;
    VkInstance instance;
    VkSurfaceKHR surface;


    VkRenderPass renderPass;
    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFromat;
    VkExtent2D swapChainExtent;
    
    VkDevice device;
    VkPhysicalDevice physicalDevice;


    int windowWidth = 1920;
    int windowHeight = 1080;
    std::string title = "Farming sim Project";

    void InitWindow();
    void InitVulkan();


    void CreateSwapChain();
    

    void MainLoop();

    void CleanUp();

private:
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    QueueFamilyIndices indices;

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    QueueFamilyIndices FindQueueFamilies();

    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkImage> swapChainImages;
    
private:
   
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice PHdevice);

    VkSurfaceFormatKHR ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilitys);

    bool isDeviceSuitable(VkPhysicalDevice device);


    void CreateImageViews();

    void CreateRenderPass();

//shaders
private:
    VkShaderModule fragShaderModule;
    VkShaderModule vertShaderModule;

    std::vector<char> ReadFile(const std::string& fileName);

    VkShaderModule CreateShaderModule(const std::vector<char>& code);

    void CreateGraphicsPipline();   


    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

//frame buffers
private:

    VkCommandPool commandPool;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkFramebuffer> swapChainFramebuffers;

    void CreateFrameBuffers();

    void CreateCommandPool();

    void CreateCommandBuffers();

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;

    VkFence inFlightFence;

    void InitSyncIbjects();

    void DrawFrame();

};

