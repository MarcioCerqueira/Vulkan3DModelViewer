#include "AmbientOcclusionApplication.h"

AmbientOcclusionApplication::AmbientOcclusionApplication(const int windowWidth, const int windowHeight)
{
	const std::string applicationName{ "Ambient Occlusion Application" };
	window = std::make_unique<Window>(windowWidth, windowHeight, applicationName);
	graphicsInstance = std::make_unique<GraphicsInstance>(applicationName);
	graphicsInstance->createWindowSurface(window->getGLFWWindow());
	graphicsInstance->selectPhysicalDevice();
	graphicsInstance->createLogicalDevice(window->getFramebufferSize());
	graphicsInstance->createGraphicsPipeline(loadShaders(graphicsInstance->getVulkanLogicalDevice()));
}

std::vector<std::shared_ptr<Shader>> AmbientOcclusionApplication::loadShaders(const vk::Device& vulkanLogicalDevice) const
{
	std::vector<std::shared_ptr<Shader>> shaders;
	shaders.push_back(std::make_shared<Shader>("shaders/vert.spv", vulkanLogicalDevice, vk::ShaderStageFlagBits::eVertex));
	shaders.push_back(std::make_shared<Shader>("shaders/frag.spv", vulkanLogicalDevice, vk::ShaderStageFlagBits::eFragment));
	return shaders;
}

void AmbientOcclusionApplication::run() const
{
	window->open();
}