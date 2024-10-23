#include "Headers/FarmApp.h"


FarmApp::FarmApp()
{
    CreatePipelineLayout();
    CreateUIPipelineLayout();
    CreatePipeline();
    CreateUIPipeline();
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
		time.UpdateDeltaTime();
      	glfwPollEvents();
        DrawFrame();
    }

    vkDeviceWaitIdle(device.GetDevice());
    
}


void FarmApp::CreateUIPipelineLayout()
{
	// Create the pipeline layout
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(glm::mat4);  // For orthographic projection matrix

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;


	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 0;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    samplerLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &samplerLayoutBinding;

	VkDescriptorSetLayout descriptorSetLayout;
	if (vkCreateDescriptorSetLayout(device.GetDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
    	throw std::runtime_error("Failed to create descriptor set layout!");
	}


    if (vkCreatePipelineLayout(device.GetDevice(), &pipelineLayoutInfo, nullptr, &uiPipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create UI pipeline layout!");
    }
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




void FarmApp::CreateUIPipeline()
{
	PipelineConfigInfo pipelineConfig{};
	Pipeline::DefaulfPipelineConfigInfo(
		pipelineConfig,
		swapChain.width(),
		swapChain.height()
	);
	pipelineConfig.renderPass = swapChain.getRenderPass();
	pipelineConfig.pipelineLayout = uiPipelineLayout;

	std::string vertPath = "../src/Shaders/vert_UI_Shader.vert.spv";
    std::string fragPath = "../src/Shaders/frag_UI_Shader.frag.spv";

	uiPipeline = std::make_unique<Pipeline>(
		device,
		fragPath, 
		vertPath, 
		pipelineConfig,
		PipeLineLayer::UIShader);
}

void FarmApp::CreatePipeline()
{
  	PipelineConfigInfo pipelineConfig{};
  	Pipeline::DefaulfPipelineConfigInfo(
      pipelineConfig,
      swapChain.width(),
      swapChain.height());
  	pipelineConfig.renderPass = swapChain.getRenderPass();
  	pipelineConfig.pipelineLayout = pipelineLayout;

    std::string vertPath = "../src/Shaders/vert_gen_shader.vert.spv";
    std::string fragPath = "../src/Shaders/frag_gen_shader.frag.spv";

  	pipeline = std::make_unique<Pipeline>(
      device,
      fragPath,
      vertPath,
      pipelineConfig,
	  PipeLineLayer::GraphicsShader);
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

    glm::mat4 orthoMatrix = glm::ortho(0.0f, (float)window.GetWindowWidth(), 0.0f, (float)window.GetWindowHeight());
	vkCmdPushConstants(commandBuffers[i], uiPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(orthoMatrix), &orthoMatrix);

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

	//std::cout << "FPS: " << time.GetFPS() << "\n";

    result = swapChain.SubmitCommandBuffer(&commandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image!");
    }
}

