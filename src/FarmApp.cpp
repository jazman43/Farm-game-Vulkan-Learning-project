#include "Headers/FarmApp.h"


FarmApp::FarmApp()
{
    CreatePipelineLayout();
    CreatePipline();
    CreateCommandBuffers();
}

FarmApp::~FarmApp()
{
  vkDestroyPipelineLayout(device.GetDevice(), pipelineLayout, nullptr);
}

void FarmApp::Run()
{
    while (!window.ShouldClose())
    {
        glfwPollEvents();
        DrawFrame();
    }

    vkDeviceWaitIdle(device.GetDevice());
    
}

void FarmApp::CreatePipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(device.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
          VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}


void FarmApp::CreatePipline()
{
  PipelineConfigInfo pipelineConfig{};
  Pipeline::DefaulfPipelineConfigInfo(
      pipelineConfig,
      swapChain.width(),
      swapChain.height());
  pipelineConfig.renderPass = swapChain.getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;

    std::string vertPath = "/media/jared/Projects/Vulkan_Projects/Farm_Project/Farm_game_Vulkan_Learning_project/src/Shaders/vert_gen_shader.vert.spv";
    std::string fragPath = "/media/jared/Projects/Vulkan_Projects/Farm_Project/Farm_game_Vulkan_Learning_project/src/Shaders/frag_gen_shader.frag.spv";

  pipeline = std::make_unique<Pipeline>(
      device,
      fragPath,
      vertPath,
      pipelineConfig);
}

void FarmApp::CreateCommandBuffers()
{
  commandBuffers.resize(swapChain.imageCount());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = device.GetCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(device.GetDevice(), &allocInfo, commandBuffers.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }

  for (int i = 0; i < commandBuffers.size(); i++) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain.getRenderPass();
    renderPassInfo.framebuffer = swapChain.getFrameBuffer(i);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    

    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkBuffer vertexBuffers[] = {device.GetVertextBuffer()}; // Your vertex buffer
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

    pipeline->Bind(commandBuffers[i]);
    vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffers[i]);
    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }
  }
}


void FarmApp::DrawFrame()
{
    uint32_t imageIndex;
    auto result = swapChain.AcpuireNextImage(&imageIndex);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to acquire swap chain image!");
    }

    //std::cout << "Image Index: " << imageIndex << " / " << swapChain.imageCount() << std::endl;
    if (imageIndex >= swapChain.imageCount())
    {
      throw std::runtime_error("Invalid image index!");
    }

    result = swapChain.SubmitCommandBuffer(&commandBuffers[imageIndex], &imageIndex);//when we run this we get an error
    if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image!");
    }
}

