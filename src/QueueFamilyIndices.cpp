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
	std::ranges::for_each(queueFamilies, [this, &queueFamilyIndex](const auto& queueFamily) {
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
		{
			graphicsFamilyIndex = queueFamilyIndex;
		}
		++queueFamilyIndex;
	});
}

void QueueFamilyIndices::computeValidPresentFamilyIndex(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface)
{
	std::vector<uint32_t> queueFamilyIndices(vulkanPhysicalDevice.getQueueFamilyProperties().size());
	std::ranges::generate(queueFamilyIndices, [queueFamilyIndex = 0] () mutable { return queueFamilyIndex++; });
	std::ranges::for_each(queueFamilyIndices, [this, vulkanPhysicalDevice, vulkanWindowSurface](const auto queueFamilyIndex){
		if (vulkanPhysicalDevice.getSurfaceSupportKHR(queueFamilyIndex, vulkanWindowSurface))
		{
			presentFamilyIndex = queueFamilyIndex;
		}
	});
}

const std::optional<uint32_t>& QueueFamilyIndices::getGraphicsFamilyIndex() const
{
	return graphicsFamilyIndex;
}

const std::optional<uint32_t>& QueueFamilyIndices::getPresentFamilyIndex() const
{
	return presentFamilyIndex;
}
