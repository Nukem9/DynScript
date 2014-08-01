#pragma once

namespace Script
{
namespace GUI
{
	void Init(asIScriptEngine *Engine);

	// AngelScript functions
	int asAddSubmenu(int Handle, std::string &Title);
	bool asAddMenuEntry(int Handle, int Entry, std::string &Title);
	bool asAddMenuSeparator(int MenuHandle);
	bool asClearMenu(int Handle);
}
}