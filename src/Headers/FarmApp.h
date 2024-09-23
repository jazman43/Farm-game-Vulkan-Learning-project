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
    uint32_t graphicsFamily;
    uint32_t presentFamily;

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };


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

    std::vector<char> ReadFile(const std::string& fileName);

    VkShaderModule CreateShaderModule(const std::vector<char>& code);

    void CreateGraphicsPipline();

    std::vector<char> fragGenShaderCode = ReadFile("/media/jared/progets/vulkan projects/Project Farm/build/Bin/Shaders/frag_gen_shader.spv");
    std::vector<char> vertGenShaderCode = ReadFile("/media/jared/progets/vulkan projects/Project Farm/build/Bin/Shaders/vert_gen_shader.spv");

    VkShaderModule fragShaderModule = CreateShaderModule(fragGenShaderCode);
    VkShaderModule vertShaderModule = CreateShaderModule(vertGenShaderCode);


    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

};

