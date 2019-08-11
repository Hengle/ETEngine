#pragma once
#include <EtCore/Helper/Singleton.h>

#include "AssetDatabase.h"
#include "AssetPointer.h"


// forward decl
class I_Package;


//---------------------------------
// ResourceManager
//
// Class that manages the lifetime of assets
//
class ResourceManager : public Singleton<ResourceManager>
{
public:
	// Definitions
	//---------------------

	friend class Singleton<ResourceManager>;

	static constexpr char s_DatabasePath[] = "asset_database.json";

	typedef std::pair<T_Hash, I_Package*> T_IndexedPackage;

private:
	// Construct destruct
	//---------------------
	ResourceManager() = default;
	~ResourceManager();
	// Protect from copy construction
	ResourceManager(const ResourceManager& t);
	ResourceManager& operator=(const ResourceManager& t);

public:
	// Init Deinit
	//---------------------
	void InitFromCompiledData();

	void Deinit();

	// Accessors
	//---------------------
	bool IsUnloadDeferred() const { return m_DeferUnloadToFlush; }
	I_Package* GetPackage(T_Hash const id);

	// Managing assets
	//---------------------
	template <class T_DataType>
	AssetPtr<T_DataType> GetAssetData(T_Hash const assetId);

	void Flush(); 

	// Utility
	//---------------------
private:
	template <class T_DataType>
	RawAsset<T_DataType>* GetAsset(T_Hash const assetId);


	// Data
	///////

	AssetDatabase m_Database;
	std::vector<T_IndexedPackage> m_Packages;

	bool m_DeferUnloadToFlush = false;
};


#include "ResourceManager.inl"