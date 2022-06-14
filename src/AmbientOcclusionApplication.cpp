#include "AmbientOcclusionApplication.h"

AmbientOcclusionApplication::AmbientOcclusionApplication(const int windowWidth, const int windowHeight)
{
	const std::string applicationName{ "Ambient Occlusion Application" };
	window = std::make_unique<Window>(windowWidth, windowHeight, applicationName);
	graphicsInstance = std::make_unique<GraphicsInstance>(applicationName);
	graphicsInstance->createWindowSurface(window->getGLFWWindow());
	graphicsInstance->selectPhysicalDevice();
	graphicsInstance->createLogicalDevice(window->getFramebufferSize());
	loadGraphicsPipeline();
}

void AmbientOcclusionApplication::loadGraphicsPipeline()
{
	const vk::Device vulkanLogicalDevice = graphicsInstance->getVulkanLogicalDevice();
	const Shader vertexShader("shaders/vert.spv", vulkanLogicalDevice, vk::ShaderStageFlagBits::eVertex);
	const Shader fragmentShader("shaders/frag.spv", vulkanLogicalDevice, vk::ShaderStageFlagBits::eFragment);
	const RenderPass renderPass(vulkanLogicalDevice, graphicsInstance->getSwapChainSurfaceFormat());
	GraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
	graphicsPipelineCreateInfo.vulkanLogicalDevice = vulkanLogicalDevice;
	graphicsPipelineCreateInfo.swapChainExtent = graphicsInstance->getSwapChainExtent();
	graphicsPipelineCreateInfo.shaderStages.push_back(vertexShader.buildPipelineShaderStageCreateInfo());
	graphicsPipelineCreateInfo.shaderStages.push_back(fragmentShader.buildPipelineShaderStageCreateInfo());
	graphicsPipelineCreateInfo.vulkanRenderPass = renderPass.getVulkanRenderPass();
	const GraphicsPipeline graphicsPipeline(graphicsPipelineCreateInfo);
}

void AmbientOcclusionApplication::run() const noexcept
{
	window->open();
}