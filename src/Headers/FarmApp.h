#pragma once


#include <glm/glm.hpp>


#include <iostream>
#include <memory>
#include <string.h>
#include <array>

#include "devices.h"
#include "pipeline.h"
#include "window.h"
#include "swapChain.h"




class FarmApp
{
public:
    FarmApp();

    FarmApp(const FarmApp &) = delete;
    FarmApp &operator=(const FarmApp &) = delete;

    ~FarmApp();

    void Run();

private:
    //create our glfw window
    Window window;
    //create device
    Device device{window};
    //create swap chain
    SwapChain swapChain{device, window.GetExtent()};
    //pipeline 
    std::unique_ptr<Pipeline> pipeline;

    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
   
    void CreatePipelineLayout();
    void CreatePipline();
    void CreateCommandBuffers();   

    void DrawFrame();

   
};

