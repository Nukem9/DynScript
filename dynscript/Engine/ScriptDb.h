#pragma once

namespace Script
{
namespace Db
{
	bool LoadModule(const char *FilePath);
	bool UnloadModule(const char *Name);
	const ModuleDef *FindModule(const char *Name);
	void DbUnloadAll();
	bool ForeachModule(std::function<bool(const ModuleDef *Module)> Callback);
}
}