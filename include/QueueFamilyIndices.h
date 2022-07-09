#pragma once

#include <vulkan/vulkan.hpp>
#include <optional>

class QueueFamilyIndices
{
public:
	QueueFamilyIndices(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface);
	const std::optional<uint32_t> getGraphicsFamilyIndex() const;
	const std::optional<uint32_t> getPresentFamilyIndex() const;

private:
	void computeValidGraphicsFamilyIndex(const vk::PhysicalDevice& vulkanPhysicalDevice);
	void computeValidPresentFamilyIndex(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface);

	std::optional<uint32_t> graphicsFamilyIndex;
	std::optional<uint32_t> presentFamilyIndex;
};