#pragma once

namespace Engine
{
	class IStreamWriter;
	class IStreamReader;

	/// <summary>
	/// An Interface that can be considered serializable.
	/// </summary>
	class ISerializable
	{
	public:
		virtual bool Deserialize(IStreamReader& reader) =0;
		virtual bool Serialize(IStreamWriter& writer) =0;
	};
}