#include "../stdafx.h"

namespace Script
{
namespace Db
{
	std::vector<ModuleDef> dbModules;

	bool LoadModule(const char *FilePath)
	{
		// Request engine to load script
		ModuleDef def;

		if (EngineLoad(&def, FilePath))
			return false;

		// Insert this script to the global array of handles
		dbModules.push_back(def);
		return true;
	}

	bool UnloadModule(const char *Name)
	{
		auto foundModule = std::find_if(dbModules.begin(), dbModules.end(),
		[Name](const ModuleDef& Item)
		{
			// Match internal AngelScript name
			return _stricmp(Name, Item.asModule->GetName()) == 0;
		});

		// Was this specific module found?
		if (foundModule == dbModules.end())
			return false;

		// Remove from the vector and free all module resources
		foundModule->asModule->Discard();
		dbModules.erase(foundModule);
		return true;
	}

	const ModuleDef *FindModule(const char *Name)
	{
		auto foundModule = std::find_if(dbModules.begin(), dbModules.end(),
		[Name](const ModuleDef& Item)
		{
			// Match internal AngelScript name
			return _stricmp(Name, Item.asModule->GetName()) == 0;
		});

		// Was this specific module found?
		if (foundModule == dbModules.end())
			return false;

		return &(*foundModule);
	}

	void DbUnloadAll()
	{
		// Unload all modules as a loop
		ForeachModule([](const ModuleDef *Module)
		{
			Module->asModule->Discard();
			return true;
		});

		// Empty the vector
		dbModules.clear();
	}

	bool ForeachModule(std::function<bool (const ModuleDef *Module)> Callback)
	{
		// Internally loop through the vector and send it to the callback
		for (auto& module : dbModules)
		{
			if (!Callback(&module))
				return false;
		}

		return true;
	}
}
}