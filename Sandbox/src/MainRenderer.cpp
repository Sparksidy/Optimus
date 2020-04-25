#include <pch.h>
#include "MainRenderer.h"

#include <Optimus/Graphics/RenderStage.h>
#include <Optimus/Graphics/Subrenders/SubRender2D.h>

MainRenderer::MainRenderer()
{
	std::vector<OP::Attachment> renderpassAttachment = {
		{0, "swapchain", OP::Attachment::Type::Swapchain}
	};

	std::vector<OP::SubpassType> renderpassSubpasses = {
		{0, {0}}
	};

	OP::Renderer::AddRenderStage(renderpassAttachment, renderpassSubpasses);
}
void MainRenderer::Start()
{
	AddSubRender<OP::SubRender2D>({0, 0});
} 
void MainRenderer::Update()
{
}