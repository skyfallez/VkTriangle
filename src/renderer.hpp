#pragma once

#include <cstdio>
#include <cstring>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan.h"
#include "vulkan_renderer_types.hpp"

#define VK_CHECK(expr)                                             \
{                                                                  \
    if(expr != VK_SUCCESS){                                        \
        printf( "Not vkSuccess '%s' : '%d'\n", __FILE__, __LINE__);\
        exit(1);                                                   \
    }                                                              \
}                                                                  \

class Renderer{

    private:
        HWND m_hwnd;
        HINSTANCE m_hInstance;

        VkInstance m_instance;
        VkSurfaceKHR m_surface;
        SAdapter m_adapter;
        SDevice m_device;

        std::vector<const char*> m_instanceLayers;
        std::vector<const char*> m_instanceExtensions;
        std::vector<const char*> m_deviceExtensions;
        
        void CreateInstance();
        void CreateSurface();
        void CreateAdapter();
        void CreateDevice();

        bool CheckQueueFamilies(VkPhysicalDevice& adapter, std::vector<VkQueueFamilyProperties>& queueFamilyProperties);
        bool CheckDeviceExtensions(std::vector<VkExtensionProperties>& extensionsProperties);



    public:
        Renderer(HWND hwnd, HINSTANCE hInstance);
        ~Renderer();

        void Initialize();
        void Update();
        void Draw();

};