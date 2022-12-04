#pragma once
#include "veDevice.hpp"
#include <fstream>
#include <vector>
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
		vePipeline(veDevice& vedevice,VkExtent2D swapChainExtent, VkRenderPass& renderPass);
		void loadShader();
		void createPipeline();
		void cleanPipeline();
		VkShaderModule  createShaderModule(const std::vector<char>& code);
		veDevice& vedevice;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
		VkPipelineLayout pipelineLayout;
		VkRenderPass& renderPass;
		VkPipeline graphicsPipeline;
		VkExtent2D swapChainExtent;
	};
}
#endif