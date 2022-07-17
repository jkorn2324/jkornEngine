#include "EnginePCH.h"
#include "RefCounters.h"

namespace Engine
{

	// ---------------------------- Implicit Reference Counter -------------------------------


	ImplicitRefCounter::ImplicitRefCounter()
		: m_referenceCounter(new CounterPtr(1)), m_containsReferenceCounter(true)
	{
	}

	ImplicitRefCounter::ImplicitRefCounter(const ImplicitRefCounter& counter)
	{
		if (m_containsReferenceCounter)
		{
			m_referenceCounter->numReferences--;
		}
		m_referenceCounter = counter.m_referenceCounter;
		m_containsReferenceCounter = counter.m_containsReferenceCounter;
		if (m_containsReferenceCounter) m_referenceCounter->numReferences++;
	}

	ImplicitRefCounter::ImplicitRefCounter(const IRefCounter& counter)
	{
		if (m_containsReferenceCounter)
		{
			m_referenceCounter->numReferences--;
		}
		m_referenceCounter = counter.GetRawReferenceCounter();
		m_containsReferenceCounter = counter.ContainsValidReferenceCounter();
		if (m_containsReferenceCounter) m_referenceCounter->numReferences++;
	}

	ImplicitRefCounter::~ImplicitRefCounter()
	{
		if (m_containsReferenceCounter)
		{
			m_referenceCounter->numReferences--;
			if (m_referenceCounter->numReferences <= 0)
			{
				delete m_referenceCounter;
				m_referenceCounter = nullptr;
				m_containsReferenceCounter = false;
			}
		}
	}

	ImplicitRefCounter& ImplicitRefCounter::operator=(const ImplicitRefCounter& counter)
	{
		if (m_containsReferenceCounter)
		{
			m_referenceCounter->numReferences--;
		}
		m_referenceCounter = counter.m_referenceCounter;
		m_containsReferenceCounter = counter.m_containsReferenceCounter;
		if (m_containsReferenceCounter) m_referenceCounter->numReferences++;
		return *this;
	}
}