#pragma once

#include <string>
#include "HashMap.h"
#include "RTTI.h"

namespace FieaGameEngine
{
	class Asset : public RTTI
	{
		RTTI_DECLARATIONS(Asset, RTTI)

	public:

		static const std::string TYPE_MESH;
		static const std::string TYPE_TEXTURE;
		static const std::string TYPE_VERTEX_SHADER;
		static const std::string TYPE_PIXEL_SHADER;

	public:

		Asset();

		virtual ~Asset();

		virtual void Load(char* data,
						  std::uint32_t size,
						  const std::string& path);

		const std::string& GetPath() const;

		const std::string& GetName() const;

		static bool Load(const std::string& path,
						 const std::string& name,
						 const std::string& type);

		static Asset* Get(const std::string& name);

		static bool Unload(const std::string& name);

		static void UnloadAll();

	private:

		std::string mPath;

		std::string mName;

		static HashMap<std::string, Asset*> sAssetMap;
	};
}