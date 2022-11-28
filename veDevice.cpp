#include "veDevice.hpp"
#include <iostream>
#include <unordered_set>
#include <set>

namespace vengin {
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(//�ص�����
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}
	VkResult CreateDebugUtilsMessengerEXT(//�����ص�
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

	void DestroyDebugUtilsMessengerEXT(//���ٻص�
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

	veDevice::veDevice(veWindow &window, veSwapChain& swapChain):window(window),swapChain(swapChain) {
		hasGflwRequiredInstanceExtensions();//�ж���Ҫ����չ�Ƿ�֧��
		createInstance();//����ʵ��
		setupDebugMessenger();//����У���ص�
		createSurface();//������Ⱦ����
		pickPhysicalDevice();//ѡ����ʵ������豸
		createLogicalDevice();//�����߼��豸
		createCommandPool();//���������
		createSwapChain();
	}
	veDevice::~veDevice()
	{
		vkDestroySwapchainKHR(device_, swapChain.swapChain, nullptr);
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
		//�����debugģʽ,�Ϳ���У���
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
	//������Ҫ����չ
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
	//�ж���Ҫ����չ�Ƿ�֧��
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
	//�ж���Ҫ����֤���Ƿ�֧��
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
	//����debug�ṹ��
	void veDevice::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |//��Ҫ���ú�������Ϣ����:����ʹ���
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |//��Ϣ����
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;//���õĻص�
		createInfo.pUserData = nullptr;  // Optional
	}
	//���ûص�
	void veDevice::setupDebugMessenger()
	{
		if (!enableValidationLayers) { return; }
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		populateDebugMessengerCreateInfo(createInfo);
		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("fail to set debugCallback");
		}
	}
	//ѡ�������豸
	void veDevice::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);//��ȡ�����豸����
		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}
		std::cout << "Device count: " << deviceCount << std::endl;//�������
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());//��ȡ�����豸��Ϣ
		//ͨ���Զ��庯��Ѱ�ҷ��������������豸
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
	//�����߼��豸
	void veDevice::createLogicalDevice()
	{
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);//��ȡ�����������

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;//�������еĽṹ��,����¼����
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };//д������,set����ȥ��

		float queuePriority = 1.0f;//���ȼ�
		for (uint32_t queueFamily : uniqueQueueFamilies) {//ѭ�����ṹ����Ϣ
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;//�����������
			queueCreateInfo.queueCount = 1;//�����Ķ�������
			queueCreateInfo.pQueuePriorities = &queuePriority;//���ȼ�
			queueCreateInfos.push_back(queueCreateInfo);//д������
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};//�豸����
		deviceFeatures.samplerAnisotropy = VK_TRUE;//��������������

		VkDeviceCreateInfo createInfo = {};//�߼��豸�����ṹ��
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());//������
		createInfo.pQueueCreateInfos = queueCreateInfos.data();//��������

		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());//��չ��
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();//��չ

		//�ж��Ƿ�����У���
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		//�����߼��豸
		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device_) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}
		//��������,ͼ�ζ��кͳ��ֶ���;ͨ���������������ȡ�����о��
		vkGetDeviceQueue(device_, indices.graphicsFamily, 0, &graphicsQueue_);
		vkGetDeviceQueue(device_, indices.presentFamily, 0, &presentQueue_);
	}
	void veDevice::createSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = swapChain.chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = swapChain.chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = swapChain.chooseSwapExtent(swapChainSupport.capabilities);
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 &&
			imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface_;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
		uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily,
						(uint32_t)indices.presentFamily };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;
		if (vkCreateSwapchainKHR(device_, &createInfo, nullptr,
			&swapChain.swapChain) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}
	}
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
	//�ж������豸�Ƿ����
	bool veDevice::isDeviceSuitable(VkPhysicalDevice device)
	{
		//n2 ��ȡ�����豸��ϸ����
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		std::cout << "physical device: " << deviceProperties.deviceName << std::endl;
		//n2 ��ȡ�豸����
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		return deviceProperties.deviceType ==
			VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU &&
			deviceFeatures.geometryShader;
	}
	//Ѱ�ҿ��ö�����
	QueueFamilyIndices veDevice::findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;//�����ṹ��
		//��ȡ�����豸���ڵĶ����������
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		//��ȡ�����豸�ϵĶ��������Ϣ
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
		//����,Ѱ�ҿ��õĶ����岢����������¼��ṹ����
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
	//��ѯ��Ⱦ����Խ�������֧����
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