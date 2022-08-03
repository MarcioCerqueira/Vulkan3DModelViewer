#include "AmbientOcclusionApplication.h"

AmbientOcclusionApplication::AmbientOcclusionApplication(const int windowWidth, const int windowHeight) : window(windowWidth, windowHeight, applicationName), graphicsInstance(applicationName)
{
	graphicsInstance.createWindowSurface(window.getGLFWWindow());
	graphicsInstance.selectPhysicalDevice();
	graphicsInstance.createLogicalDevice(window.getFramebufferSize(), vertices, indices);
	graphicsInstance.createGraphicsPipeline(loadShaders(graphicsInstance.getVulkanLogicalDevice()));
}

const std::vector<std::shared_ptr<Shader>> AmbientOcclusionApplication::loadShaders(const vk::Device& vulkanLogicalDevice) const
{
	const std::vector<std::shared_ptr<Shader>> shaders = {
		std::make_shared<Shader>("shaders/vert.spv", vulkanLogicalDevice, vk::ShaderStageFlagBits::eVertex),
		std::make_shared<Shader>("shaders/frag.spv", vulkanLogicalDevice, vk::ShaderStageFlagBits::eFragment)
	};
	return shaders;
}

void AmbientOcclusionApplication::run()
{
	std::function<void(std::function<WindowSize()>, std::function<void()>)> drawFrame = std::bind(&GraphicsInstance::drawFrame, &graphicsInstance, std::placeholders::_1, std::placeholders::_2);
	window.open(drawFrame);
	graphicsInstance.waitIdle();
}