#include "pch.h"
#include "LayerStack.h"

namespace OP
{

	LayerStack::LayerStack()
	{
		m_Layerinserter = begin();
	}

	LayerStack::~LayerStack()
	{
		for (auto layer : m_LayerStack)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layerinserter = m_LayerStack.emplace(m_Layerinserter, layer); //Create a new layer at the inserter pos and update the inserter pos
	}

	void LayerStack::PopLayer(Layer * layer)
	{
		auto it = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);
		if (it != end())
		{
			m_LayerStack.erase(it); //Shifts all the elements after deleting the one- EXPENSIVE as it copies all the element over.
			m_Layerinserter--;
		}
	}

	void LayerStack::PushOverlay(Layer * layer)
	{
		m_LayerStack.emplace_back(layer);
	}

	void LayerStack::PopOverlay(Layer * layer)
	{
		auto it = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);
		if (it != end())
		{
			m_LayerStack.erase(it);
		}
	}


}

