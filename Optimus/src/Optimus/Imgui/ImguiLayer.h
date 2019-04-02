#pragma once
#include "Optimus/Core.h"
#include "Optimus/Layer.h"

namespace OP
{
	class ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();

		virtual void OnAttach()override;
		virtual void OnDetach()override;
		virtual void OnUpdate()override;
		virtual void OnEvent(Event&)override;
	private:

	};
}