#include <pch.h>
#include "MainRenderer.h"

#include <Optimus/Graphics/Core/RenderStage.h>
#include <Optimus/Graphics/Subrenders/SubRender2D.h>

#include <Optimus/Application.h>

MainRenderer::MainRenderer()
{
	std::vector<OP::Attachment> renderpassAttachment = {
		{0, "swapchain", OP::Attachment::Type::Swapchain}
	};

	std::vector<OP::SubpassType> renderpassSubpasses = {
		{0, {0}}
	};

	OP::Renderer::AddRenderStage(std::make_unique<RenderStage>(renderpassAttachment, renderpassSubpasses));
}
void MainRenderer::Start()
{
	AddSubRender<OP::SubRender2D>({0, 0});
} 
void MainRenderer::Update()
{
	/*
		If window is resized, recreate Graphics Pipeline. (Expensive)

		TODO: Create Dynamic States in Vulkan and just change the scissor and viewport information. (Cheap)
	*/
	if (OP::Application::Get().IsWindowResized())
	{
		OP_CORE_INFO("Window is resized");

		ClearSubRenders();

		AddSubRender<OP::SubRender2D>({ 0, 0 });

		//Set isWindow Resized back to false
		OP::Application::Get().ResetWindowResize();
	}
}