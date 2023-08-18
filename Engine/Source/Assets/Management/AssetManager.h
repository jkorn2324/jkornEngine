#pragma once

namespace Engine
{
	class AssetManager
	{
	public:

		/**
		 * Initializes the assets.
		 */
		static void Initialize();

		/**
		 * Called to refresh the assets inside of the asset manager.
		 */
		static void RefreshAssets();
	};
}