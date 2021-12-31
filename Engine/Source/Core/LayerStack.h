#pragma once

#include <vector>

namespace Engine
{


	class LayerStack
	{
	public:
		explicit LayerStack();
		~LayerStack();

		void Clear();

		void AddLayer(class Layer* windowLayer);
		void AddOverlay(class Layer* windowLayer);
		void RemoveLayer(class Layer* windowLayer);
		void RemoveOverlay(class Layer* windowLayer);

		std::vector<class Layer*>::const_iterator begin() const { return m_layerStack.begin(); }
		std::vector<class Layer*>::const_iterator end() const { return m_layerStack.end(); }
		std::vector<class Layer*>::const_reverse_iterator rbegin() const { return m_layerStack.rbegin(); }
		std::vector<class Layer*>::const_reverse_iterator rend() const { return m_layerStack.rend(); }

	private:
		std::vector<class Layer*> m_layerStack;
		std::uint32_t m_overlayInsertIndex;
	};
}