#pragma once
#include "veWindow.hpp"
#include "veDevice.hpp"
#include "veSwapChain.hpp"
#include "veRender.hpp"
//#include "vePipeline.hpp"
//#include "veRender.hpp"
namespace vengin {
	const int MAX_FRAMES_IN_FLIGHT = 2;
	class applic
	{
	public:
		applic();
		~applic();
		//禁止拷贝构造
		applic(const applic&) = delete;
		applic& operator=(const applic&) = delete;
		//void setPipelineConfig(pipelineConfigInfo& config);
		//void createPipelineLayout();
		void run();
		void draw();
		void createSyncObjects();
		void clean();
	private:
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		size_t currentFrame = 0;
		//bool framebufferResized = false;
		//veSwapChain vSwapchain;
		veWindow vWindow{800,600,"hello vulkan"};
		veDevice vDevice{ vWindow };
		veRender vRender{ vDevice };
		//vePipeline vPipeline;//写函数定义吧
		//VkPipelineLayout pipelineLayout;
		//beginFrame->beginRender->endRender->endFrame这样子   
		
	};
}
