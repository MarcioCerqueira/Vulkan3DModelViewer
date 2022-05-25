#include "SwapChain.h"

SwapChain::SwapChain(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface, const WindowSize& framebufferSize)
{
    vk::SurfaceCapabilitiesKHR capabilities{ vulkanPhysicalDevice.getSurfaceCapabilitiesKHR(vulkanWindowSurface) };
    const std::vector<vk::SurfaceFormatKHR> availableFormats{ vulkanPhysicalDevice.getSurfaceFormatsKHR(vulkanWindowSurface) };
    std::vector<vk::PresentModeKHR> availablePresentModes{ vulkanPhysicalDevice.getSurfacePresentModesKHR(vulkanWindowSurface) };

    checkSwapChainValidity(availableFormats, availablePresentModes);
    chooseSwapSurfaceFormat(availableFormats);
    chooseSwapPresentMode(availablePresentModes);
    chooseSwapExtent(capabilities, framebufferSize);
    setImageCount(capabilities);
}

void SwapChain::checkSwapChainValidity(const std::vector<vk::SurfaceFormatKHR>& availableFormats, const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    valid = !availableFormats.empty() && !availablePresentModes.empty();
}

void SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
    format = availableFormats[0];
    for (const auto& availableFormat : availableFormats) 
    {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) 
        {
            format = availableFormat;
            return;
        }
    }
}

void SwapChain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    presentMode = vk::PresentModeKHR::eFifo;
    for (const auto& availablePresentMode : availablePresentModes) 
    {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) 
        {
            presentMode = availablePresentMode;
            return;
        }
    }
}

void SwapChain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const WindowSize& framebufferSize)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        extent = capabilities.currentExtent;
    }
    else
    {
        extent = vk::Extent2D{ static_cast<uint32_t>(framebufferSize.width), static_cast<uint32_t>(framebufferSize.height) };
        extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }
}

void SwapChain::setImageCount(const vk::SurfaceCapabilitiesKHR& capabilities)
{
    imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) 
    {
        imageCount = capabilities.maxImageCount;
    }
}

bool SwapChain::isValid() const noexcept
{
    return valid;
}