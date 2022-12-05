#pragma once
#include "veWindow.hpp"
namespace vengin {
	struct QueueFamilyIndices {
		uint32_t graphicsFamily;//֧�ֻ��ƵĶ�����
		uint32_t presentFamily;//֧�ֱ��ֵö�����
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;
		bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
	};
	struct SwapChainSupportDetails {//������֧�ֵ�ϸ��
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
		//��ֹ����
		veDevice(veDevice&) = delete;
		veDevice& operator=(veDevice&) = delete;
		VkDevice getDevice() { return device_; }
		SwapChainSupportDetails getSwapchainSupprts() { return querySwapChainSupport(physicalDevice); }//���ؽ������Ĵ���ϸ��
		QueueFamilyIndices findPhysicalFamilies() { return findQueueFamilies(physicalDevice); }//���ض���������� n
		VkCommandPool getCommandPool() { return commandPool; }
		VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }
		VkQueue getGraphicsQueue() { return graphicsQueue_; }
		VkQueue getPresentQueue() { return presentQueue_; }
		void createInstance();//����ʵ��
		//obj
		VkSurfaceKHR surface_;//��Ⱦ����,����GLFW��õĿɹ�Vulkan��Ⱦ�ı���
		veWindow& window;
		//VkDescriptorSetLayout descriptorSetLayout;//����������
		//void createDescriptorSetLayout();//��������������
	private:
		//object
		VkInstance instance;//ʵ��
		VkDebugUtilsMessengerEXT debugMessenger;//debug����
		VkDevice device_;//�߼��豸
		
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;//�����豸,��ʼ��λ��
		//���ڶ���
		//veSwapChain& swapChain;//������
		VkQueue graphicsQueue_, presentQueue_;//��Ⱦ����ֶ���
		VkCommandPool commandPool;//�����
		//function
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);//�õ������غ��ʵĶ����������
		std::vector<const char*> getRequiredExtensions();//��ȡ��չ
		void hasGflwRequiredInstanceExtensions();//�ж��Ƿ�֧����Ҫ����չ
		bool checkValidationLayerSupport();//�ж��Ƿ�֧��������֤��
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);//���ص�����������Ľṹ��
		void setupDebugMessenger();//���ûص�
		void pickPhysicalDevice();//ѡȡ�����豸
		void createSurface();//������Ⱦ����
		void createLogicalDevice();//�����߼��豸
		//void createSwapChain();
		//void createSwapChainImageViews();
		void createCommandPool();
		bool isDeviceSuitable(VkPhysicalDevice device);//ɸѡ�����豸
		
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);//��ȡ��Ⱦ����֧�ֵĹ���
		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };//�����Ҫ��У���
		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };//��Ҫ���豸��չ
	};
}