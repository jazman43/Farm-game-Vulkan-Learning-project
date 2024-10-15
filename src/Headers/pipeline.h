#pragma once
#include <glm/glm.hpp>

#include "devices.h"

#include <sstream>
#include <fstream>
#include <cassert>




struct PipelineConfigInfo {
  PipelineConfigInfo() = default;
  PipelineConfigInfo(const PipelineConfigInfo&) = delete;
  PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};


class Pipeline
{
public:
  Pipeline(Device &device, std::string &fragPath, std::string &vertPath, PipelineConfigInfo &pipelineInfo);
  ~Pipeline();

  void Bind(VkCommandBuffer commandBuffer);

  static void DefaulfPipelineConfigInfo(PipelineConfigInfo& configInfo, uint32_t width, uint32_t height);

private:
  Device& device;
  
  VkPipeline graphicsPipeline;
  VkShaderModule fragShaderModule;
  VkShaderModule vertShaderModule;

  void CreateGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);
  
  

  VkShaderModule CreateShaderModule(const std::vector<char>& code);

  std::vector<char> ReadFile(const std::string& fileName);
};