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

	Shader vertexShader("shaders/vert.spv", graphicsInstance->getVulkanLogicalDevice(), vk::ShaderStageFlagBits::eVertex);
	Shader fragmentShader("shaders/frag.spv", graphicsInstance->getVulkanLogicalDevice(), vk::ShaderStageFlagBits::eFragment);
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages(2);
	shaderStages[0] = vertexShader.buildPipelineShaderStageCreateInfo();
	shaderStages[1] = fragmentShader.buildPipelineShaderStageCreateInfo();

	RenderPass renderPass(graphicsInstance->getVulkanLogicalDevice(), graphicsInstance->getSwapChainSurfaceFormat());
	GraphicsPipeline graphicsPipeline(graphicsInstance->getVulkanLogicalDevice(), graphicsInstance->getSwapChainExtent(), shaderStages, renderPass.getVulkanRenderPass());
}

void AmbientOcclusionApplication::run() const noexcept
{
	window->open();
}