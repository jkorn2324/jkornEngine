#pragma once

#include <stdint.h>

namespace Engine
{

	/// <summary>
	/// Interface for reference counters;
	/// </summary>
	class IRefCounter
	{
	protected:
		struct CounterPtr
		{
			int32_t numReferences;

			CounterPtr() : numReferences(0) { }
			CounterPtr(int32_t numReferences)
				: numReferences(numReferences) { }
		};

	public:
		virtual ~IRefCounter() { }
		virtual bool IsValid() const =0;
		virtual int32_t GetReferenceCount() const =0;

	public:
		virtual bool ContainsValidReferenceCounter() const =0;
		virtual CounterPtr* GetRawReferenceCounter() const =0;
	};

	/// <summary>
	/// Implicit Reference Counter Class.
	/// 
	/// Implicitly Reference Counts the Object based on copy constructors.
	/// </summary>
	class ImplicitRefCounter : public IRefCounter
	{
	public:
		ImplicitRefCounter();
		ImplicitRefCounter(const ImplicitRefCounter& referenceCounter);
		ImplicitRefCounter(const IRefCounter& referenceCounter);
		~ImplicitRefCounter();
		
		ImplicitRefCounter& operator=(const ImplicitRefCounter& counter);

		bool IsValid() const override { return m_containsReferenceCounter 
			&& m_referenceCounter->numReferences > 0; }

		int32_t GetReferenceCount() const override
		{
			if (!m_containsReferenceCounter) return 0;
			return m_referenceCounter->numReferences;
		}

	protected:
		CounterPtr* GetRawReferenceCounter() const override { return m_referenceCounter; }
		bool ContainsValidReferenceCounter() const override { return m_containsReferenceCounter; }

	private:
		CounterPtr* m_referenceCounter;
		bool m_containsReferenceCounter;
	};

#define REF_COUNTED_OBJECT() \
	public: \
		int32_t GetReferenceCount() const { return m_referenceCounter.GetReferenceCount(); } \
		bool IsValidRefCount() const { return GetReferenceCount() > 0; } \
	private: \
		ImplicitRefCounter m_referenceCounter
}