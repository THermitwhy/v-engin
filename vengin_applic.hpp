#pragma once
#include "veWindow.hpp"
#include "veDevice.hpp"
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
		veWindow vWindow{800,600,"hello vulkan"};
		veDevice vDevice{ vWindow };
		//veRender render;
		//vePipeline vPipeline;//д���������
		//VkPipelineLayout pipelineLayout;
		//beginFrame->beginRender->endRender->endFrame������   
		
	};
}
