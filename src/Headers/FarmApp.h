#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>
#include <memory>
#include <string.h>
#include <array>
//Create render 
#include "devices.h"
#include "pipeline.h"
#include "window.h"
#include "swapChain.h"
//object perent classes
#include "UIObject.h"


//util classes
#include "Time.h"


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
    std::unique_ptr<Pipeline> uiPipeline;


    Time time;

    VkDescriptorSetLayout descriptorSetLayout;

    VkPipelineLayout pipelineLayout;
    VkPipelineLayout uiPipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
   
    void CreatePipelineLayout();
    void CreateUIPipelineLayout();
    void CreatePipeline();
    void CreateUIPipeline();
    void CreateCommandBuffers();   

    void DrawFrame();

    std::unique_ptr<UIObject> uiObject;
    
};

