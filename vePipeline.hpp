#pragma once
#define GLFW_INCLUDE_VULKAN
#include <fstream>
#include <vector>
#include <GLFW/glfw3.h>
#ifndef VE_PIPELINE
#define VE_PIPELINE

namespace vengin {
	//��ȡ�ļ�����������
	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);//ate�����ļ�β����ʼ��ȡ;binary���Զ����Ƶ���ʽ��ȡ�ļ�(�������������ĩ��ʽ��\n����\r\n��ת��)


		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);//��ת���ļ�ͷ��
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	class vePipeline {
	public:
		vePipeline(VkDevice& device,VkExtent2D swapChainExtent);
		void loadShader();
		void createPipeline();
		VkShaderModule  createShaderModule(const std::vector<char>& code);
		VkDevice& device;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
		VkPipelineLayout pipelineLayout;
		VkExtent2D swapChainExtent;
	};
}
#endif