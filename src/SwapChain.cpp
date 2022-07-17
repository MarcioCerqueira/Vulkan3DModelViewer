#include "SwapChain.h"

SwapChain::SwapChain(const SwapChainCreateInfo& swapChainCreateInfo) : swapChainCreateInfo(swapChainCreateInfo)
{
    vk::SurfaceCapabilitiesKHR capabilities{ swapChainCreateInfo.vulkanPhysicalDevice.getSurfaceCapabilitiesKHR(swapChainCreateInfo.vulkanWindowSurface) };
    const std::vector<vk::SurfaceFormatKHR> availableFormats{ swapChainCreateInfo.vulkanPhysicalDevice.getSurfaceFormatsKHR(swapChainCreateInfo.vulkanWindowSurface) };
    std::vector<vk::PresentModeKHR> availablePresentModes{ swapChainCreateInfo.vulkanPhysicalDevice.getSurfacePresentModesKHR(swapChainCreateInfo.vulkanWindowSurface) };
    chooseSwapSurfaceFormat(availableFormats);
    chooseSwapPresentMode(availablePresentModes);
    chooseSwapExtent(capabilities, swapChainCreateInfo.framebufferSize);
    buildVulkanSwapChain(swapChainCreateInfo, capabilities, estimateImageCount(capabilities));
    buildSwapChainImageViews(swapChainCreateInfo);
}

SwapChain::~SwapChain()
{
    cleanup();
}

void SwapChain::cleanup()
{
    for (int imageIndex = 0; imageIndex < imageViews.size(); imageIndex++)
    {
        framebuffers[imageIndex].reset();
        imageViews[imageIndex].reset();
    }
    swapChainCreateInfo.vulkanLogicalDevice.destroySwapchainKHR(vulkanSwapChain);
}

void SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
    surfaceFormat = availableFormats[0];
    for (const auto& availableFormat : availableFormats) 
    {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) 
        {
            surfaceFormat = availableFormat;
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

const uint32_t SwapChain::estimateImageCount(const vk::SurfaceCapabilitiesKHR& capabilities)
{
    uint32_t imageCount{ capabilities.minImageCount + 1 };
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) 
    {
        imageCount = capabilities.maxImageCount;
    }
    return imageCount;
}

const bool SwapChain::isValid(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface)
{
    const std::vector<vk::SurfaceFormatKHR> availableFormats{ vulkanPhysicalDevice.getSurfaceFormatsKHR(vulkanWindowSurface) };
    const std::vector<vk::PresentModeKHR> availablePresentModes{ vulkanPhysicalDevice.getSurfacePresentModesKHR(vulkanWindowSurface) };
    return !availableFormats.empty() && !availablePresentModes.empty();
}

void SwapChain::buildVulkanSwapChain(const SwapChainCreateInfo& swapChainCreateInfo, const vk::SurfaceCapabilitiesKHR& capabilities, const uint32_t imageCount)
{
    std::optional<uint32_t> graphicsFamilyIndex{ swapChainCreateInfo.queueFamilyIndices.getGraphicsFamilyIndex() };
    std::optional<uint32_t> presentFamilyIndex{ swapChainCreateInfo.queueFamilyIndices.getPresentFamilyIndex() };
    uint32_t queueFamilyIndices[] = { graphicsFamilyIndex.value(), presentFamilyIndex.value() };
    vk::SwapchainCreateInfoKHR swapChainCreateInfoKHR{
        .surface = swapChainCreateInfo.vulkanWindowSurface,
        .minImageCount = imageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
        .imageSharingMode = (graphicsFamilyIndex != presentFamilyIndex) ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive,
        .queueFamilyIndexCount = (graphicsFamilyIndex != presentFamilyIndex) ? static_cast<uint32_t>(2) : static_cast<uint32_t>(0),
        .pQueueFamilyIndices = (graphicsFamilyIndex != presentFamilyIndex) ? queueFamilyIndices : nullptr,
        .preTransform = capabilities.currentTransform,
        .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
        .presentMode = presentMode
    };
    vulkanSwapChain = swapChainCreateInfo.vulkanLogicalDevice.createSwapchainKHR(swapChainCreateInfoKHR);
    images = swapChainCreateInfo.vulkanLogicalDevice.getSwapchainImagesKHR(vulkanSwapChain);
}


void SwapChain::buildSwapChainImageViews(const SwapChainCreateInfo& swapChainCreateInfo)
{
    imageViews.resize(images.size());
    for (int imageIndex = 0; imageIndex < images.size(); ++imageIndex)
    {
        imageViews[imageIndex] = std::make_unique<ImageView>(swapChainCreateInfo.vulkanLogicalDevice, images[imageIndex], surfaceFormat);
    }
}

void SwapChain::buildFramebuffers(const vk::Device& vulkanLogicalDevice, const vk::RenderPass& vulkanRenderPass)
{
    framebuffers.resize(imageViews.size());
    for (int framebufferIndex = 0; framebufferIndex < framebuffers.size(); framebufferIndex++)
    {
        framebuffers[framebufferIndex] = std::make_unique<Framebuffer>(vulkanLogicalDevice, vulkanRenderPass, imageViews[framebufferIndex]->getVulkanImageView(), extent);
    }
}

vk::Result SwapChain::acquireNextImage(vk::Semaphore& imageAvailable, const vk::RenderPass& vulkanRenderPass, uint32_t& imageIndex)
{
    constexpr uint64_t timeout{ std::numeric_limits<uint64_t>::max() };
    return swapChainCreateInfo.vulkanLogicalDevice.acquireNextImageKHR(vulkanSwapChain, timeout, imageAvailable, nullptr, &imageIndex);
}

void SwapChain::recreateIfResultIsOutOfDateOrSuboptimalKHR(vk::Result& result, const SwapChainRecreateInfo& swapChainRecreateInfo)
{
    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
    {
        waitValidFramebufferSize(swapChainRecreateInfo.getFramebufferSize, swapChainRecreateInfo.waitEvents);
        swapChainCreateInfo.vulkanLogicalDevice.waitIdle();
        cleanup();
        buildVulkanSwapChain(swapChainCreateInfo, swapChainCreateInfo.vulkanPhysicalDevice.getSurfaceCapabilitiesKHR(swapChainCreateInfo.vulkanWindowSurface), static_cast<const uint32_t>(images.size()));
        buildSwapChainImageViews(swapChainCreateInfo);
        buildFramebuffers(swapChainCreateInfo.vulkanLogicalDevice, swapChainRecreateInfo.vulkanRenderPass);
        result = vk::Result::eSuccess;
    }
}

void SwapChain::waitValidFramebufferSize(std::function<WindowSize()> getFramebufferSize, std::function<void()> waitEvents)
{
    WindowSize framebufferSize = getFramebufferSize();
    while (framebufferSize.width == 0 || framebufferSize.height == 0)
    {
        framebufferSize = getFramebufferSize();
        waitEvents();
    }
}

const vk::Extent2D SwapChain::getExtent() const
{
    return extent;
}

const vk::Framebuffer SwapChain::getVulkanFramebuffer(const int framebufferIndex) const
{
    return framebuffers[framebufferIndex]->getVulkanFramebuffer();
}

const vk::SurfaceFormatKHR SwapChain::getSurfaceFormat() const
{
    return surfaceFormat;
}

const vk::SwapchainKHR SwapChain::getVulkanSwapChain() const
{
    return vulkanSwapChain;
}