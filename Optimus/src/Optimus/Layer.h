#pragma once
#include "Optimus/Core.h"

namespace OP
{
	class Event;
	class OPTIMUS_API Layer
	{
	public:
		Layer(const std::string& name) :m_LayerName(name) {}
		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event&) {}

	private:
		std::string m_LayerName;
	};
}