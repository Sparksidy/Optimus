#include <pch.h>
#include "MainRenderer.h"

#include <Optimus/Graphics/RenderStage.h>

MainRenderer::MainRenderer()
{
	std::vector<OP::Attachment> renderpassAttachment = {
		{0, "depth", OP::Attachment::Type::Depth},
		{1, "swapchain", OP::Attachment::Type::Swapchain}
	};

	std::vector<OP::SubpassType> renderpassSubpasses = {
		{0, {0, 1}}
	};

	OP::Renderer::AddRenderStage(renderpassAttachment, renderpassSubpasses);
}
void MainRenderer::Start()
{
}
void MainRenderer::Update()
{
}