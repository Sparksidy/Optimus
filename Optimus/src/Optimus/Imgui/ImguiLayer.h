#pragma once
#include "Optimus/Core.h"
#include "Optimus/Layer.h"


#include "Optimus/Events/MouseEvents.h"
#include "Optimus/Events/KeyboardEvents.h"

namespace OP
{
	class OPTIMUS_API ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();

		virtual void OnAttach()override;
		virtual void OnDetach()override;
		virtual void OnUpdate()override;
		virtual void OnEvent(Event&)override;

		bool OnMouseButtonPressedEvent(MouseButtonPressed&);
		bool OnMouseButtonReleasedEvent(MouseButtonReleased&);
		bool OnMouseScrollEvent(MouseScroll&);
		bool OnMouseMoveEvent(MouseMove&);
		bool OnKeyPressedEvent(KeyPressedEvent&);
		bool OnKeyReleasedEvent(KeyReleasedEvent&);
		bool OnKeyTypedEvent(KeyTypedEvent&);

		static void ShowSimpleOverlay(bool* p_open);

	private:
		float m_time = 0.0f;
	};
}