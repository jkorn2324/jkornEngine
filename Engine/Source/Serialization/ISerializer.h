#pragma once

namespace Engine
{
	class IStreamWriter;
	class IStreamReader;

	template<typename T>
	class ISerializer
	{
	public:
		ISerializer(T& serializedObject)
			: m_serializedObject(serializedObject) { }

		virtual ~ISerializer() { }

		virtual void Serialize(IStreamWriter& stream) =0;
		virtual void Deserialize(IStreamReader& stream) =0;

	private:
		T& m_serializedObject;
	};
}