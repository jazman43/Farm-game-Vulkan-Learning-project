#pragma once

#include "window.h"


#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <vector>
#include <optional>
#include <string.h>
#include <array>
#include <set>

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

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;    
};


class Device
{
public:
    #ifdef NDEBUG
    const bool enableValidationLayers = false;
    #else
    const bool enableValidationLayers = true;
    #endif

    Device(Window &window);

    ~Device();

    //getters
    VkCommandPool GetCommandPool() { return commandPool; }
    VkDevice GetDevice() { return device; }
    VkSurfaceKHR Getsurface() { return surface; }
    VkQueue GetGraphicsQueue() { return graphicsQueue; }
    VkQueue GetPresentQueue() { return presentQueue; }

    //
    void CreateBuffer(VkDeviceSize size, 
                     VkBufferUsageFlags usage,
                     VkMemoryPropertyFlags properties, 
                     VkBuffer& buffer,
                     VkDeviceMemory& bufferMemory);

    void CopyBuffer(VkBuffer srcBuffer, 
                    VkBuffer dstBuffer, 
                    VkDeviceSize size);

    void CreateVertexBuffer();
    //

    VkBuffer GetVertextBuffer(){    return vertexBuffer;    }

    QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(); }

    SwapChainSupportDetails GetQuerySwapChainSupport();

    VkFormat FindSupportedFormat(
      const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);


    void CreateImageWithInfo(
      const VkImageCreateInfo &imageInfo,
      VkMemoryPropertyFlags properties,
      VkImage &image,
      VkDeviceMemory &imageMemory);

      
private:
    void CreateInstance();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateCommandPool();
    void CreateSurface();
    void CreateCommandBuffers();

    
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    

    std::vector<const char * > GetRequiredExtensions();
    bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);
    void InitValidationLayers();
    bool isDeviceSuitable(VkPhysicalDevice device);


    Window window;
    VkInstance instance;
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkSurfaceKHR surface;
    VkCommandPool commandPool;

    std::vector<VkCommandBuffer> commandBuffers;


    QueueFamilyIndices indices;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    QueueFamilyIndices FindQueueFamilies();



    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

};
