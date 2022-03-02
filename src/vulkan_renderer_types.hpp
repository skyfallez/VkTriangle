#pragma once

#include "vulkan.h"

struct SAdapter{
    VkPhysicalDevice gpu;
};

struct SDevice{
    VkDevice handle;
    int graphicsIndex;
    int presentIndex;
    int transferIndex;
};