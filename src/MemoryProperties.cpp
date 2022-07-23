#include "MemoryProperties.h"

uint32_t MemoryProperties::findMemoryType(const vk::PhysicalDevice& vulkanPhysicalDevice, uint32_t memoryTypeFilter, vk::MemoryPropertyFlags memoryPropertyFlags)
{
    const vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties{ vulkanPhysicalDevice.getMemoryProperties() };
    for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if (memoryTypeFilter & (1 << i))
        {
            if ((physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)
            {
                return i;
            }
        }
    }
    throw std::runtime_error("Failed to find suitable memory type!");
}