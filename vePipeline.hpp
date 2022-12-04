#pragma once
#include "veDevice.hpp"
#include <fstream>
#include <vector>
#ifndef VE_PIPELINE
#define VE_PIPELINE

namespace vengin {
	//读取文件，返回数据
	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);//ate：从文件尾部开始读取;binary：以二进制的形式读取文件(避免进行诸如行末格式是\n还是\r\n的转换)


		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);//跳转到文件头部
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