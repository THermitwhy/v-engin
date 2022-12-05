#pragma once
#include "veWindow.hpp"
namespace vengin {
	struct QueueFamilyIndices {
		uint32_t graphicsFamily;//支持绘制的队列族
		uint32_t presentFamily;//支持表现得队列族
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;
		bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
	};
	struct SwapChainSupportDetails {//交换链支持的细节
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
	class veDevice
	{
	public:
#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif
		veDevice(veWindow &window);
		~veDevice();
		//禁止拷贝
		veDevice(veDevice&) = delete;
		veDevice& operator=(veDevice&) = delete;
		VkDevice getDevice() { return device_; }
		SwapChainSupportDetails getSwapchainSupprts() { return querySwapChainSupport(physicalDevice); }//返回交换链的创建细节
		QueueFamilyIndices findPhysicalFamilies() { return findQueueFamilies(physicalDevice); }//返回队列族的索引 n
		VkCommandPool getCommandPool() { return commandPool; }
		VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }
		VkQueue getGraphicsQueue() { return graphicsQueue_; }
		VkQueue getPresentQueue() { return presentQueue_; }
		void createInstance();//创建实例
		//obj
		VkSurfaceKHR surface_;//渲染表面,借助GLFW获得的可供Vulkan渲染的表面
		veWindow& window;
		//VkDescriptorSetLayout descriptorSetLayout;//描述符布局
		//void createDescriptorSetLayout();//创建描述符布局
	private:
		//object
		VkInstance instance;//实例
		VkDebugUtilsMessengerEXT debugMessenger;//debug代理
		VkDevice device_;//逻辑设备
		
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;//物理设备,初始化位空
		//窗口对象
		//veSwapChain& swapChain;//交换链
		VkQueue graphicsQueue_, presentQueue_;//渲染与呈现队列
		VkCommandPool commandPool;//命令池
		//function
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);//得到并返回合适的队列族的索引
		std::vector<const char*> getRequiredExtensions();//获取扩展
		void hasGflwRequiredInstanceExtensions();//判断是否支持需要的扩展
		bool checkValidationLayerSupport();//判断是否支持所需验证层
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);//填充回调代理函数所需的结构体
		void setupDebugMessenger();//设置回调
		void pickPhysicalDevice();//选取物理设备
		void createSurface();//创建渲染表面
		void createLogicalDevice();//创建逻辑设备
		//void createSwapChain();
		//void createSwapChainImageViews();
		void createCommandPool();
		bool isDeviceSuitable(VkPhysicalDevice device);//筛选物理设备
		
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);//获取渲染表面支持的功能
		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };//填充需要的校验层
		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };//需要的设备扩展
	};
}