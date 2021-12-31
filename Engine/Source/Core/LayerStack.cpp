#include "EnginePCH.h"
#include "LayerStack.h"
#include "Layer.h"

namespace Engine
{

	LayerStack::LayerStack()
		: m_layerStack(), m_overlayInsertIndex(0)
	{
	}

	LayerStack::~LayerStack()
	{
		Clear();
	}

	void LayerStack::Clear()
	{
		std::int32_t size = (std::int32_t)m_layerStack.size() - 1;
		while (size >= 0)
		{
			delete m_layerStack[size];
			size--;
		}
		m_layerStack.clear();
	}

	void LayerStack::AddLayer(Layer* windowLayer)
	{
		m_layerStack.emplace(m_layerStack.begin() + m_overlayInsertIndex, windowLayer);
		m_overlayInsertIndex++;
		windowLayer->OnLayerAdded();
	}

	void LayerStack::AddOverlay(Layer* windowLayer)
	{
		m_layerStack.push_back(windowLayer);
		windowLayer->OnLayerAdded();
	}

	void LayerStack::RemoveLayer(Layer* windowLayer)
	{
		const auto& find = std::find(m_layerStack.begin(),
			m_layerStack.begin() + m_overlayInsertIndex, windowLayer);
		if (find != m_layerStack.begin() + m_overlayInsertIndex)
		{
			windowLayer->OnLayerRemoved();
			m_layerStack.erase(find);
			m_overlayInsertIndex--;
		}
	}

	void LayerStack::RemoveOverlay(Layer* windowLayer)
	{
		const auto& find = std::find(m_layerStack.begin() + m_overlayInsertIndex,
			m_layerStack.end(), windowLayer);
		if (find != m_layerStack.end())
		{
			windowLayer->OnLayerRemoved();
			m_layerStack.erase(find);
		}
	}

}