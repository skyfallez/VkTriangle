#include "renderer.hpp"


#define _DEBUG 1

Renderer::Renderer(HWND hwnd, HINSTANCE hInstance)
    :   m_hwnd(hwnd),
        m_hInstance(hInstance),  
        m_instance(0),
        m_surface(0),
        m_adapter(),
        m_device(),
        m_instanceLayers(0),
        m_instanceExtensions(0),
        m_deviceExtensions(0) 
{
    Initialize();
}



void Renderer::Initialize()
{
    m_instanceLayers.push_back("VK_LAYER_KHRONOS_validation");
    
    m_instanceExtensions.push_back("VK_KHR_surface");
    m_instanceExtensions.push_back("VK_KHR_win32_surface");

    m_deviceExtensions.push_back("VK_KHR_swapchain");

    CreateInstance();
    CreateSurface();
    CreateAdapter();
    CreateDevice();

}

void Renderer::CreateDevice()
{
    float prio = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::vector<int> indicies;


    if(m_device.graphicsIndex == m_device.presentIndex){
        indicies.push_back(m_device.graphicsIndex);
    }else{
        indicies.push_back(m_device.graphicsIndex);
        indicies.push_back(m_device.presentIndex);
    }

    for(uint32_t i = 0; i < indicies.size(); i++){

        VkDeviceQueueCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0; 
        createInfo.queueFamilyIndex = indicies[i];
        createInfo.queueCount = 1;
        createInfo.pQueuePriorities = &prio;

        queueCreateInfos.push_back(createInfo);
    }

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = NULL;
    deviceCreateInfo.enabledExtensionCount =  m_deviceExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensions.data();

    VK_CHECK( vkCreateDevice(m_adapter.gpu, &deviceCreateInfo, NULL, &m_device.handle) ); 

    printf("Device Created");
}   

void Renderer::CreateInstance()
{
    //Get Available Instance Layers and Extensions
    uint32_t layerCount = 0;
    uint32_t extensionCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, availableExtensions.data());

    #ifdef _DEBUG
    printf("Avilable Layers\n");
    for(uint32_t i = 0; i < layerCount; i++){
        printf(" - %d : %s\n", i, availableLayers[i].layerName);
    }

    printf("Avilable Extensions\n");
    for(uint32_t i = 0; i < extensionCount; i++){
        printf(" - %d : %s\n", i, availableExtensions[i].extensionName);
    }

    // Chekc if requrested layers is available
    for(uint32_t i = 0; i < m_instanceLayers.size(); i++){
        bool found = false;

        for(uint32_t j = 0; j < layerCount; j++){

            if( strcmp(m_instanceLayers[i], availableLayers[j].layerName) == 0){
                found = true;
            }
        }

        if(!found){
            printf("Requested Layer '%s' not avilable\nExiting Program .....\n", m_instanceLayers[i]);
            exit(1);
        }
    }

    // Chekc if requrested layers is available
    for(uint32_t i = 0; i < m_instanceExtensions.size(); i++){
        bool found = false;

        for(uint32_t j = 0; j < extensionCount; j++){

            if( strcmp(m_instanceExtensions[i], availableExtensions[j].extensionName) == 0){
                found = true;
            }
        }

        if(!found){
            printf("Requested Extension '%s' not avilable\nExiting Program .....\n", m_instanceExtensions[i]);
            exit(1);
        }
    }

    #endif // _DEBUG


    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = m_instanceLayers.size();
    createInfo.ppEnabledLayerNames = m_instanceLayers.data();
    createInfo.enabledExtensionCount = m_instanceExtensions.size();
    createInfo.ppEnabledExtensionNames = m_instanceExtensions.data();

    VK_CHECK( vkCreateInstance(&createInfo, nullptr, &m_instance) );

    printf("Vulkan Instance Created\n");
}

void Renderer::CreateAdapter()
{
    uint32_t adapterCount = 0;

    vkEnumeratePhysicalDevices(m_instance, &adapterCount, NULL);
    if(adapterCount == 0){
        printf("No adpaters Found\nExiting program .....\n");
        exit(1);
    }
    std::vector<VkPhysicalDevice> adapters(adapterCount);
    vkEnumeratePhysicalDevices(m_instance, &adapterCount, adapters.data());

    bool found = false;

    printf("Enumerating Adapters .....\n");
    for(uint32_t i = 0; i < adapterCount; i++){

        uint32_t queueCount = 0;
        uint32_t deviceExtensionCount = 0;

        VkPhysicalDeviceProperties properties = {};
        VkPhysicalDeviceFeatures features = {};

        vkGetPhysicalDeviceProperties(adapters[i], &properties);
        vkGetPhysicalDeviceFeatures(adapters[i], &features);
        vkGetPhysicalDeviceQueueFamilyProperties(adapters[i], &queueCount, NULL);
        vkEnumerateDeviceExtensionProperties(adapters[i], NULL, &deviceExtensionCount, NULL);

        std::vector<VkExtensionProperties> deviceExtensions(deviceExtensionCount);
        std::vector<VkQueueFamilyProperties> queueFamProperties(queueCount);

        vkEnumerateDeviceExtensionProperties(adapters[i], NULL, &deviceExtensionCount, deviceExtensions.data());
        vkGetPhysicalDeviceQueueFamilyProperties(adapters[i], &queueCount, queueFamProperties.data());

        printf("%s\n", properties.deviceName);

        bool queueFamiliesSupport = CheckQueueFamilies(adapters[i], queueFamProperties);
        bool deviceExtensionSupport = CheckDeviceExtensions( deviceExtensions);

        if(queueFamiliesSupport && deviceExtensionSupport){
            m_adapter.gpu = adapters[i];
            found = true;
        }

    }

    if(!found){
        printf("No Adapter is supported\nExiting program ...\n");
        exit(1);
    }

    printf("Adapter Created\n");
}

bool Renderer::CheckDeviceExtensions(std::vector<VkExtensionProperties>& extensionsProperties)
{
    bool support = true;
    printf("\t- Available Device Extensions\n");
    int len = extensionsProperties.size();
    printf("%d", len);

    #ifdef _DEBUG
    for(int i = 0; i < len; i++){
        printf("\t\t %d : '%s'\n",i , extensionsProperties[i].extensionName);
    }
    #endif //_DEBUG

    for(uint32_t i = 0; i < m_deviceExtensions.size(); i++){
        bool found = false;

        for(uint32_t j = 0; j < extensionsProperties.size(); j++){
            if(strcmp(m_deviceExtensions[i], extensionsProperties[j].extensionName) == 0){
                found = true;
            }
        }

        if(!found){
            printf("\t Device Extension '%s' not found\n", m_deviceExtensions[i]);
            support = false;
        }
    }
    return support;
}

bool Renderer::CheckQueueFamilies(VkPhysicalDevice& adapter, std::vector<VkQueueFamilyProperties>& queueFamilyProperties)
{
        bool graphicsFound = false;
        bool presentFound = false;
        bool transferFound = false;
        VkBool32 present;

        int graphicsIndex;
        int presentIndex;
        int transferIndex;

        int gpSHARE = -1;

    for(uint32_t i = 0; i < queueFamilyProperties.size(); i++){

        #ifdef _DEBUG
        printf("\t- QueueFamilyIndex '%d' | ", i);
        #endif //_DEBUG
        
        if(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            graphicsFound = true;
            graphicsIndex = i;
            printf("graphics YES | ");
        }else{
            printf(" graphics NO | ");
        }

        if(queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT){
            transferFound = true;
            transferIndex = i;
            printf(" transfer YES | ");
        }else{
            printf(" transfer NO | ");
        }

        vkGetPhysicalDeviceSurfaceSupportKHR(adapter, i, m_surface, &present);
        if(present){
            presentFound = true;
            presentIndex = i;
            printf(" present YES | ");
        }else{
            printf(" present NO | ");

        }

        if(graphicsIndex != -1 && (graphicsIndex == presentIndex)){
            gpSHARE = i;
            break;
        }
        if(transferFound) m_device.transferIndex = transferIndex;
    }

    if(!graphicsFound || !presentFound){
        printf(" -- NO --\n");
        return false;
    }else{
        printf(" -- YES --\n");

    }

    if(gpSHARE != -1){
        m_device.graphicsIndex = gpSHARE;
        m_device.presentIndex = gpSHARE;

    }else{
        m_device.graphicsIndex = graphicsIndex;
        m_device.presentIndex = presentIndex;
    }

    return true;
}

void Renderer::CreateSurface()
{
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = m_hwnd;
    createInfo.hinstance = m_hInstance;
    VK_CHECK( vkCreateWin32SurfaceKHR(m_instance, &createInfo, NULL, &m_surface) );
    printf("Vulkan SurfaceKHR Created\n");
}

void Renderer::Update()
{
   
}

void Renderer::Draw()
{


}

Renderer::~Renderer()
{
}