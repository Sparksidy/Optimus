#pragma once
#include "Optimus/Layer.h"

/*		LAYERSTACK
	|------------------|
	|LAYER   | OVERLAYS|
	|------------------|
*/

namespace OP
{
	typedef std::vector<Layer* >::iterator LayerVectorIterator;

	class OPTIMUS_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void PushOverlay(Layer* layer);
		void PopOverlay(Layer* layer);

		LayerVectorIterator begin() { return m_LayerStack.begin(); }
		LayerVectorIterator end() { return m_LayerStack.end(); }

	private:
		std::vector<Layer* >m_LayerStack;
		LayerVectorIterator m_Layerinserter;		//Keeps track of pushing overlays or layers in the layerstack
	};
}


