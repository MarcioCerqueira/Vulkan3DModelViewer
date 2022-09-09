#include "QueueFamilyIndices.h"

QueueFamilyIndices::QueueFamilyIndices(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface)
{
	computeValidGraphicsFamilyIndex(vulkanPhysicalDevice);
	computeValidPresentFamilyIndex(vulkanPhysicalDevice, vulkanWindowSurface);
}

void QueueFamilyIndices::computeValidGraphicsFamilyIndex(const vk::PhysicalDevice& vulkanPhysicalDevice)
{
	int queueFamilyIndex{ 0 };
	const std::vector<vk::QueueFamilyProperties> queueFamilies{ vulkanPhysicalDevice.getQueueFamilyProperties() };
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
		{
			graphicsFamilyIndex = queueFamilyIndex;
			break;
		}
		++queueFamilyIndex;
	}
}

void QueueFamilyIndices::computeValidPresentFamilyIndex(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface)
{
	size_t queueFamilySize{ vulkanPhysicalDevice.getQueueFamilyProperties().size() };
	for (int queueFamilyIndex = 0; queueFamilyIndex < queueFamilySize; ++queueFamilyIndex)
	{
		if (vulkanPhysicalDevice.getSurfaceSupportKHR(queueFamilyIndex, vulkanWindowSurface))
		{
			presentFamilyIndex = queueFamilyIndex;
			break;
		}
	}
}

const std::optional<uint32_t>& QueueFamilyIndices::getGraphicsFamilyIndex() const
{
	return graphicsFamilyIndex;
}

const std::optional<uint32_t>& QueueFamilyIndices::getPresentFamilyIndex() const
{
	return presentFamilyIndex;
}
