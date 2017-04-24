#include "pch.h"
#include "Asset.h"
#include "Utils.h"
#include "Factory.h"

// Asset types
#include "MeshGeometry.h"
#include "Texture.h"
#include "Shader.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Asset)

	const std::string Asset::ASSET_TYPE_MESH = "mesh";
	const std::string Asset::ASSET_TYPE_TEXTURE = "texture";
	const std::string Asset::ASSET_TYPE_SHADER = "shader";

	HashMap<std::string, Asset*> Asset::sAssetMap;

	Asset::Asset()
	{

	}

	Asset::~Asset()
	{

	}

	const std::string& Asset::GetPath() const
	{
		return mPath;
	}

	const std::string& Asset::GetName() const
	{
		return mName;
	}

	void Asset::Load(char* data)
	{
		data;
	}

	bool Asset::Load(const std::string& path,
					 const std::string& name,
					 const std::string& type)
	{
		char* fileData = nullptr;
		std::uint32_t fileSize = 0U;
		fileData = DesktopUtils::ReadFile(path, fileSize);

		if (fileData != nullptr)
		{
			// The target file was found!
			// Instantiate an asset based on the type and then 
			// add it to the static Asset map. First create factories.

			// TODO: Figure out how to get factory to work
			MeshGeometryFactory meshFactory;
			TextureFactory textureFactory;
			ShaderFactory shaderFactory;

			Asset* asset = Factory<Asset>::Create(type);
			
			if (asset != nullptr)
			{
				asset->Load(fileData);
				sAssetMap[name] = asset;
			}
			
			delete fileData;
			fileData = nullptr;

			return true;
		}

		return false;
	}

	Asset* Asset::Get(const std::string& name)
	{
		auto it = sAssetMap.Find(name);

		if (it != sAssetMap.end())
		{
			return it->second;
		}

		return nullptr;
	}

	bool Asset::Unload(const std::string& name)
	{
		auto it = sAssetMap.Find(name);

		if (it != sAssetMap.end())
		{
			// TODO: Remove double lookup with Find and then Remove.
			delete it->second;
			it->second = nullptr;
			sAssetMap.Remove(name);

			return true;
		}

		return false;
	}

	void Asset::UnloadAll()
	{
		for (HashMap<std::string, Asset*>::Iterator it = sAssetMap.begin(); it != sAssetMap.end(); ++it)
		{
			delete it->second;
			it->second = nullptr;
		}

		sAssetMap.Clear();
	}
}