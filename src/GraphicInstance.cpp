#include "GraphicInstance.h"

GraphicInstance::GraphicInstance(const std::string& applicationName)
{

	uint32_t glfwExtensionCount = 0;
	const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	const vk::ApplicationInfo applicationInfo{ applicationName.c_str(), 1, "No Engine", 1, VK_API_VERSION_1_0 };
	const vk::InstanceCreateInfo createInfo{ {}, &applicationInfo, 0, nullptr, glfwExtensionCount, glfwExtensions};
	vulkanInstance = vk::createInstance(createInfo);

}

GraphicInstance::~GraphicInstance()
{
	vulkanInstance.destroy();
}