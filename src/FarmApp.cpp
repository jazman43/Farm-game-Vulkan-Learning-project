#include "Headers/FarmApp.h"





void FarmApp::Run()
{
    InitWindow();
    InitVulkan();
    MainLoop();
    CleanUp();
}

void FarmApp::InitWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


    window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), nullptr, nullptr);

}

void FarmApp::InitVulkan()
{
       
    VkApplicationInfo appInfo{};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    appInfo.pApplicationName = "Farm-Project";
    appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);//verinton / major / minor/ pacth
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3; 

    //GLFW extensions 
    uint32_t glfwExtensionCount  =0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;


    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        std::cerr << "Failed to create Vulkan Instance!! (Farm APP .CPP)\n";
    }else
    {
        std::cout << "Vulkan Instance Created!\n";
    }
    //create surface

    if(glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    {
        std::cerr << "Failed to create window surface\n";
    }else
    {
        std::cout << "window surface Created!\n";
    }
    //physical device

    // Device extension check and creation
    

    
    
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if(deviceCount == 0)
    {
        std::cerr << "Failed to to find supported devices!\n";
    }
    else
    {
        std::cout << "Found : " << deviceCount << " device('s) with vulkan support\n";

        std::vector<VkPhysicalDevice> devices(deviceCount);

        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        //select first device

        physicalDevice = devices[0];
    }


    //set up logical device and queues

    VkDeviceQueueCreateInfo queueCreateInfo{};
    
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0;
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo deviceCreateInfo{};

    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS)
    {
        std::cerr << "failed to create logical Device!\n" ;
    }
    else
    {
        std::cout << "Logical Device Created!\n";
    }

    
    if (!isDeviceSuitable(physicalDevice)) {
        std::cerr << "Physical device doesn't support required extensions!\n" ;
    }
    else{
        std::cout << "Physical device supports required extenstions!!\n";
    }
    

    CreateSwapChain();

    CreateImageViews();

    CreateRenderPass();

    CreateGraphicsPipline();

}

void FarmApp::CreateSwapChain()
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapChainSurfaceFormat(swapChainSupport.formats);

    VkPresentModeKHR presentMode = ChooseSwapChainPresentMode(swapChainSupport.presentModes);

    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);


    uint32_t imageCount = swapChainSupport.capabilities.minImageCount +1;
    if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo{};

    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = surface;
    swapChainCreateInfo.minImageCount = imageCount;
    swapChainCreateInfo.imageFormat = surfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent = extent;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


    uint32_t queueFamilyIndices[] = {graphicsFamily, presentFamily};

    if(graphicsFamily != presentFamily)
    {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount  = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }else
    {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainCreateInfo.queueFamilyIndexCount = 0;
        swapChainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    swapChainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.clipped = VK_TRUE;

    if(vkCreateSwapchainKHR(device, &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS)
    {
        std::cerr << "Failed to create swap chain!\n";
    }else
    {
        std::cout << "Swap Chain Created!!\n";
    }


    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);

    swapChainImages.resize(imageCount);//this gives error no match for call to ‘(std::vector<VkImage_T*>) (uint32_t&)’

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFromat = surfaceFormat.format;
    swapChainExtent = extent;
}


void FarmApp::MainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
    
    
    vkDeviceWaitIdle(device);
    
    
    
}

void FarmApp::CleanUp()
{
    vkDestroySwapchainKHR(device, swapChain, nullptr);
      
    for(auto imageView : swapChainImageViews)
    {
        vkDestroyImageView(device, imageView, nullptr);
    }
    
    vkDestroyRenderPass(device, renderPass, nullptr);


    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    

    
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
    
    
    glfwDestroyWindow(window);

    glfwTerminate();
}



///


///

SwapChainSupportDetails FarmApp::querySwapChainSupport(VkPhysicalDevice PHdevice)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PHdevice, surface, &details.capabilities);

    uint32_t formatCount;

    vkGetPhysicalDeviceSurfaceFormatsKHR(PHdevice, surface, &formatCount, nullptr);

    if(formatCount != 0)
    {
        details.formats.resize(formatCount);

        vkGetPhysicalDeviceSurfaceFormatsKHR(PHdevice, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount; 

    vkGetPhysicalDeviceSurfacePresentModesKHR(PHdevice, surface, &presentModeCount, nullptr);

    if(presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);

        vkGetPhysicalDeviceSurfacePresentModesKHR(PHdevice, surface, &presentModeCount, details.presentModes.data());

    }

    return details;
}


VkSurfaceFormatKHR FarmApp::ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for(const auto& availableFormat : availableFormats)
    {
        if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR FarmApp::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
    for(const auto& availablePresentMode : availablePresentModes)
    {
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D FarmApp::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilitys)
{
    if(capabilitys.currentExtent.width != UINT32_MAX)
    {
        return capabilitys.currentExtent;
    }else
    {
        VkExtent2D actualExtent = {(uint32_t)windowWidth, (uint32_t)windowHeight};

        actualExtent.width = std::max(capabilitys.minImageExtent.width, std::min(capabilitys.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilitys.minImageExtent.height, std::min(capabilitys.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}



bool FarmApp::isDeviceSuitable(VkPhysicalDevice device) {
    // Check for device extension support
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();  // Returns true if all required extensions are supported
}



void FarmApp::CreateImageViews()
{
    swapChainImageViews.resize(swapChainImages.size());

    for(size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        VkImageViewCreateInfo imageCreateInfo{};

        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageCreateInfo.image = swapChainImages[i];
        imageCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageCreateInfo.format = swapChainImageFromat;

        imageCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        imageCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageCreateInfo.subresourceRange.baseMipLevel = 0;
        imageCreateInfo.subresourceRange.levelCount = 1;
        imageCreateInfo.subresourceRange.baseArrayLayer = 1;
        imageCreateInfo.subresourceRange.layerCount = 1;

        if(vkCreateImageView(device, &imageCreateInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
        {
            std::cerr << "Failed to create image views!!\n";
        }
        else
        {
            std::cout << "Image View: " << i << " is created!!\n";
        }
    }
}


void FarmApp::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment{};

    colorAttachment.format = swapChainImageFromat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; //no multisampling 
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


    VkSubpassDescription subPass{};
    subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPass.colorAttachmentCount = 1;
    subPass.pColorAttachments = &colorAttachmentRef;


    VkRenderPassCreateInfo renderPassInfo{};

    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subPass;


    if(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass))
    {
        std::cerr << "Failed to create render pass!!\n";
    }else
    {
        std::cout << "render pass created!!\n";
    }
}



std::vector<char> FarmApp::ReadFile(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::ate | std::ios::binary);


    if(!file.is_open())
    {
        throw std::runtime_error("FAILED to open shader file");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VkShaderModule FarmApp::CreateShaderModule(const std::vector<char> &code)
{
    VkShaderModuleCreateInfo shaderModuleInfo{};

    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleInfo.codeSize = code.size();
    shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    
    VkShaderModule shaderModule;

    if(vkCreateShaderModule(device, &shaderModuleInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        std::cerr << "FAILED to create Shader Module!\n";
    }
    else
    {
        std::cout << "Shader Module Created\n";
    }    
    
    return shaderModule;
}

void FarmApp::CreateGraphicsPipline()
{

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};

    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main"; //entry point name;


    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};


    //

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;


    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;  // Draw triangles
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChainExtent.width;
    viewport.height = (float) swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    //rasterizer

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    //multisampling (currently not using now)
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;


    //color blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    //create pipline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        std::cerr << "Failed to create pipeline layout!\n";
    }
    else
    {
        std::cout << "Pipeline layout Created!\n";
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;

    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
    {
       std::cerr << "Failed to create graphics pipeline!\n";
    }
    else
    {
        std::cout << "Graphics Pipeline Created!!\n";
    }

    vkDestroyShaderModule(device, vertShaderModule, nullptr);
    vkDestroyShaderModule(device, fragShaderModule, nullptr);
}
