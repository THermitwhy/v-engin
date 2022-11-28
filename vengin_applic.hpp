#pragma once
#include "veWindow.hpp"
#include "veDevice.hpp"
#include "veSwapChain.hpp"
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
	private:
		veSwapChain vSwapchain;
		veWindow vWindow{800,600,"hello vulkan"};
		veDevice vDevice{ vWindow,vSwapchain };
		//veRender render;
		//vePipeline vPipeline;//д���������
		//VkPipelineLayout pipelineLayout;
		//beginFrame->beginRender->endRender->endFrame������   
		
	};
}
