#include <pch.h>
#include "MainRenderer.h"

#include <Optimus/Graphics/RenderStage.h>

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
} 
void MainRenderer::Update()
{
}