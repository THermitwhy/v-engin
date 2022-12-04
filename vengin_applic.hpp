#pragma once
#include "veWindow.hpp"
#include "veDevice.hpp"
#include "veSwapChain.hpp"
#include "veRender.hpp"
//#include "vePipeline.hpp"
//#include "veRender.hpp"
namespace vengin {
	class applic
	{
	public:
		applic();
		~applic();
		//��ֹ��������
		applic(const applic&) = delete;
		applic& operator=(const applic&) = delete;
		//void setPipelineConfig(pipelineConfigInfo& config);
		//void createPipelineLayout();
		void run();
		void draw();
		void createSemaphores();
	private:
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		//veSwapChain vSwapchain;
		veWindow vWindow{800,600,"hello vulkan"};
		veDevice vDevice{ vWindow };
		veRender vRender{ vDevice };
		//vePipeline vPipeline;//д���������
		//VkPipelineLayout pipelineLayout;
		//beginFrame->beginRender->endRender->endFrame������   
		
	};
}
