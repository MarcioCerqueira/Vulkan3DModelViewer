#pragma once

#include <optional>

struct QueueFamilyIndicesModel
{
	std::optional<uint32_t> graphicsFamilyIndex;
	std::optional<uint32_t> presentFamilyIndex;
};