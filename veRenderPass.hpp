#pragma once
#include "veWindow.hpp"

namespace vengin {
	class veRenderPass {
	public:
		veRenderPass(VkFormat swapChainImageFormat);
		void createRenderPass();
		VkFormat swapChainImageFormat;
	};
}
