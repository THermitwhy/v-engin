#pragma once
#include "veDevice.hpp"
#include "veSwapChain.hpp"
#include "vertexes.hpp"

#include <stb_image.h>
namespace vengin {
	class veRender {
	public:
		veRender(veDevice& vedevice);
		void createSwapChain();
		void recreateSwapChain();
		void createCommandPool();
		void createCommandBuffers();
		
		void clean();
		void cleanUpVeSwapChain();
		void updateVeSwapChain();

		void createTextureImage();//����������ͼ
		VkImageView textureImageView;//����������ͼ������ͼ
		VkImage textureImage;//���ڴ洢�����ͼ�����
		VkSampler textureSampler;//����������
		void createTextureImageView();//���ڴ���ͼ��������ͼ
		void createTextureSampler();//����ͼ��Ĳ�����
		VkDeviceMemory textureImageMemory;//Ϊͼ����󿪱ٵ��ڴ�
		void createImage(uint32_t width, uint32_t height, VkFormat
			format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties, VkImage& image,
			VkDeviceMemory& imageMemory);//���ڴ���ͼ�����򻯲���
		    VkImageView createImageView(VkImage image, VkFormat format);//��ͼ����ͼ����Ĵ���
		void transitionImageLayout(VkImage image, VkFormat format,
			VkImageLayout oldLayout, VkImageLayout newLayout);//����ͼ��Ĳ��ֱ任
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t
			width, uint32_t height);

		void createVertexBuffer();//�������㻺��
		void createIndexBuffer();//������������
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);//����������
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);//�����ݴ�Դ���������Ƶ�Ŀ�껺����

		void createUniformBuffer();
		void updateUniformBuffer(uint32_t currentImage);

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);//Ѱ���ʺϵ��ڴ�����
		veDevice& vedevice;
		veSwapChain *veswapchain;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;


		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		void createDescriptorPool();
		void createDescriptorSets();

		//����ָ���
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	};
}
