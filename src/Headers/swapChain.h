#pragma once

#include "devices.h"

#include <vulkan/vulkan.h>


class SwapChain
{
public:

    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;
    SwapChain(const SwapChain &) = delete;
    void operator=(const SwapChain &) = delete;

    SwapChain(Device &deviceRef, VkExtent2D windowExtent);
    ~SwapChain();

    VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
    VkRenderPass getRenderPass() { return renderPass; }
    VkImageView getImageView(int index) { return swapChainImageViews[index]; }
    size_t imageCount() { return swapChainImages.size(); }
    VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
    VkExtent2D getSwapChainExtent() { return swapChainExtent; }
    uint32_t width() { return swapChainExtent.width; }
    uint32_t height() { return swapChainExtent.height; }


    VkResult AcpuireNextImage(uint32_t *imageIndex);

    VkResult SubmitCommandBuffer(const VkCommandBuffer *buffers, uint32_t *imageIndex);


private:
    void CreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateFrameBuffers();
    void CreateDepthResources();
    void InitSyncIbjects();

    VkRenderPass renderPass;
    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    Device &device;
    VkExtent2D windowExtent;

    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkImage> swapChainImages;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;

    uint32_t currentFrame = 0;
    
    std::vector<VkImage> depthImages;
    std::vector<VkDeviceMemory> depthImageMemorys;
    std::vector<VkImageView> depthImageViews;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    std::vector<VkFramebuffer> swapChainFramebuffers;


    VkSurfaceFormatKHR ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilitys);

    VkFormat FindDepthFormat();
};