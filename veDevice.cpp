#include "veDevice.hpp"
#include <iostream>
#include <unordered_set>
#include <set>

namespace vengin {
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(//回调函数
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}
	VkResult CreateDebugUtilsMessengerEXT(//创建回调
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger) {
		//we should find the addess of debugUtils function to use it
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			instance,
			"vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(//销毁回调
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			instance,
			"vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	veDevice::veDevice(veWindow &window):window(window){
		hasGflwRequiredInstanceExtensions();//判断需要的扩展是否被支持
		createInstance();//创建实例
		setupDebugMessenger();//设置校验层回调
		createSurface();//创建渲染表面
		pickPhysicalDevice();//选择合适的物理设备
		createLogicalDevice();//创建逻辑设备
		//createDescriptorSetLayout();
		//createCommandPool();//创建命令池
		//createSwapChain();
		//createSwapChainImageViews();
	}
	veDevice::~veDevice()
	{
		//swapChain.cleanImageViews(device_);
		////vkDestroySwapchainKHR(device_, swapChain.swapChain, nullptr);
		vkDestroyCommandPool(device_, commandPool, nullptr);
		vkDestroySurfaceKHR(instance, surface_, nullptr);
		vkDestroyDevice(device_, nullptr);
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		vkDestroyInstance(instance,nullptr);
	}
	void veDevice::createInstance()
	{
		if (enableValidationLayers &&
			!checkValidationLayerSupport()) {//Determine whether the required verification layer exists
			throw std::runtime_error("validation layers requested, but not available!");
		}
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		

		
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		//next 3 lines to write the extensions of GLFW for for our instance 
		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
		//如果是debug模式,就开启校验层
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("fail to create instance!");
		}
	}
	//void veDevice::createDescriptorSetLayout()
	//{
	//	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	//	uboLayoutBinding.binding = 0;
	//	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//	uboLayoutBinding.descriptorCount = 1;
	//	//descriptorCount成员变量用来指定数组中元素的个数。我们可以使用数组来指定骨骼动画使用的所有变换矩阵。
	//	//在这里，我们的MVP矩阵只需要一个uniform缓冲对象，所以我们将descriptorCount的值设置为1。
	//	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;//指定该描述符的使用场景，比如在顶点着色器中使用
	//	uboLayoutBinding.pImmutableSamplers = nullptr; // 图像采样相关，当前为默认

	//	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	//	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	//	layoutInfo.bindingCount = 1;
	//	layoutInfo.pBindings = &uboLayoutBinding;

	//	if (vkCreateDescriptorSetLayout(device_, &layoutInfo, nullptr,
	//		&descriptorSetLayout) != VK_SUCCESS) {
	//		throw std::runtime_error("failed to create descriptor set layout!");
	//	}
	//}
	//返回需要的扩展
	std::vector<const char*> veDevice::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		for (auto tension : extensions) {
			std::cout << tension << std::endl;
		}
		

		return extensions;
	}
	//判断需要的扩展是否被支持
	void veDevice::hasGflwRequiredInstanceExtensions()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::cout << "available extensions:" << std::endl;
		std::unordered_set<std::string> available;
		for (const auto& extension : extensions) {
			std::cout << "\t" << extension.extensionName << std::endl;
			available.insert(extension.extensionName);
		}

		std::cout << "required extensions:" << std::endl;
		auto requiredExtensions = getRequiredExtensions();
		for (const auto& required : requiredExtensions) {
			std::cout << "\t" << required << std::endl;
			if (available.find(required) == available.end()) {
				throw std::runtime_error("Missing required glfw extension");
			}
		}
	}
	//判断需要的验证层是否被支持
	bool veDevice::checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {//
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}
	//设置debug结构体
	void veDevice::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |//需要调用函数的消息类型:警告和错误
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |//消息类型
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;//调用的回调
		createInfo.pUserData = nullptr;  // Optional
	}
	//设置回调
	void veDevice::setupDebugMessenger()
	{
		if (!enableValidationLayers) { return; }
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		populateDebugMessengerCreateInfo(createInfo);
		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("fail to set debugCallback");
		}
	}
	//选择物理设备
	void veDevice::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);//获取物理设备数量
		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}
		std::cout << "Device count: " << deviceCount << std::endl;//输出数量
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());//获取物理设备信息
		//通过自定义函数寻找符合条件的物理设备
		for (const auto& device : devices) {
			if (isDeviceSuitable(device)) {
				physicalDevice = device;
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("failed to find a suitable GPU!");
		}
		
	}
	void veDevice::createSurface()
	{
		window.createWindowSurface(instance, &surface_);
	}
	//创建逻辑设备
	void veDevice::createLogicalDevice()
	{
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);//获取队列族的索引

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;//创建队列的结构体,容器录入多个
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };//写入索引,set可以去重

		float queuePriority = 1.0f;//优先级
		for (uint32_t queueFamily : uniqueQueueFamilies) {//循环填充结构体信息
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;//队列族的索引
			queueCreateInfo.queueCount = 1;//创建的队列数量
			queueCreateInfo.pQueuePriorities = &queuePriority;//优先级
			queueCreateInfos.push_back(queueCreateInfo);//写入容器
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};//设备特性
		deviceFeatures.samplerAnisotropy = VK_TRUE;//采样器各向异性

		VkDeviceCreateInfo createInfo = {};//逻辑设备创建结构体
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());//队列数
		createInfo.pQueueCreateInfos = queueCreateInfos.data();//队列数据

		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());//扩展数
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();//扩展

		//判断是否开启了校验层
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		//创建逻辑设备
		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device_) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}
		//创建队列,图形队列和呈现队列;通过队列族的索引获取到队列句柄
		vkGetDeviceQueue(device_, indices.graphicsFamily, 0, &graphicsQueue_);
		vkGetDeviceQueue(device_, indices.presentFamily, 0, &presentQueue_);
	}
	//void veDevice::createSwapChain()
	//{
	//	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

	//	VkSurfaceFormatKHR surfaceFormat = swapChain.chooseSwapSurfaceFormat(swapChainSupport.formats);
	//	VkPresentModeKHR presentMode = swapChain.chooseSwapPresentMode(swapChainSupport.presentModes);
	//	VkExtent2D extent = swapChain.chooseSwapExtent(swapChainSupport.capabilities);
	//	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	//	if (swapChainSupport.capabilities.maxImageCount > 0 &&
	//		imageCount > swapChainSupport.capabilities.maxImageCount) {
	//		imageCount = swapChainSupport.capabilities.maxImageCount;
	//	}
	//	VkSwapchainCreateInfoKHR createInfo = {};
	//	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	//	createInfo.surface = surface_;

	//	createInfo.minImageCount = imageCount;
	//	createInfo.imageFormat = surfaceFormat.format;
	//	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	//	createInfo.imageExtent = extent;
	//	createInfo.imageArrayLayers = 1;
	//	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	//	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	//	uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily,
	//					(uint32_t)indices.presentFamily };

	//	if (indices.graphicsFamily != indices.presentFamily) {
	//		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	//		createInfo.queueFamilyIndexCount = 2;
	//		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	//	}
	//	else {
	//		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//		createInfo.queueFamilyIndexCount = 0; // Optional
	//		createInfo.pQueueFamilyIndices = nullptr; // Optional
	//	}
	//	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	//	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	//	createInfo.presentMode = presentMode;
	//	createInfo.clipped = VK_TRUE;
	//	createInfo.oldSwapchain = VK_NULL_HANDLE;
	//	if (vkCreateSwapchainKHR(device_, &createInfo, nullptr,
	//		&swapChain.swapChain) != VK_SUCCESS) {
	//		throw std::runtime_error("failed to create swap chain!");
	//	}
	//	swapChain.swapChainExtent = extent;
	//	swapChain.swapChainImageFormat = createInfo.imageFormat;
	//}
	/*void veDevice::createSwapChainImageViews()
	{
		swapChain.createImageViews(device_);
	}*/
	void veDevice::createCommandPool()
	{
		QueueFamilyIndices indices = findPhysicalFamilies();
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = indices.graphicsFamily;
		poolInfo.flags = 0;

		if (vkCreateCommandPool(device_, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			throw std::runtime_error("fail to create commandPool!");
		}
	}
	//判断物理设备是否合适
	bool veDevice::isDeviceSuitable(VkPhysicalDevice device)
	{
		//n2 获取物理设备详细属性
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		std::cout << "physical device: " << deviceProperties.deviceName << std::endl;
		//n2 获取设备特性
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		return deviceProperties.deviceType ==
			VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU &&
			deviceFeatures.geometryShader&& deviceFeatures.samplerAnisotropy;
	}
	//寻找可用队列族
	QueueFamilyIndices veDevice::findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;//创建结构体
		//获取物理设备存在的队列族的数量
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		//获取物理设备上的队列族的信息
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
		//遍历,寻找可用的队列族并将它的索引录入结构体中
		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
				indices.graphicsFamilyHasValue = true;
			}
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_, &presentSupport);
			if (queueFamily.queueCount > 0 && presentSupport) {
				indices.presentFamily = i;
				indices.presentFamilyHasValue = true;
			}
			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}
	//查询渲染表面对交换链的支持项
	SwapChainSupportDetails veDevice::querySwapChainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(
				device,
				surface_,
				&presentModeCount,
				details.presentModes.data());
		}
		return details;
	}
}