#pragma once
#include "veWindow.hpp"
#include <glm/glm.hpp>
#include <array>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
namespace vengin {
	struct Vertex {
		glm::vec2 pos;
		glm::vec3 color;

		static VkVertexInputBindingDescription
			getBindingDescription() {//描述顶点数据的存放方式
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);//连续数据之间的跨度
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;//以一个顶点数据作为条目的粒度

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2>
			getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2>
				attributeDescriptions = {};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);
			return attributeDescriptions;
		}
	};

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};
	const std::vector<Vertex> vertices = {
					{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
					{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
					{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};
	const std::vector<uint16_t> indices = {
				0, 1, 2, 2, 3, 0
	};
}
