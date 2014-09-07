#include "../stdafx.h"

namespace Script
{
namespace GUI
{
	void Init(asIScriptEngine *Engine)
	{
		// Register all GUI variables and functions under "GUI"
		Engine->SetDefaultNamespace("GUI");

		//
		// VARIABLES
		//
		VERIFY(Engine->RegisterGlobalProperty("const int MenuHandle", &hMenu));

		//
		// FUNCTIONS
		//
		VERIFY(Engine->RegisterGlobalFunction("int AddSubmenu(int Handle, string &in Title)", asFUNCTION(asAddSubmenu), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool AddMenuEntry(int Handle, int Entry, string &in Title)", asFUNCTION(asAddMenuEntry), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool AddMenuSeparator(int Handle)", asFUNCTION(asAddMenuSeparator), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool ClearMenu(int Handle)", asFUNCTION(asClearMenu), asCALL_CDECL));
	}

	int asAddSubmenu(int Handle, std::string &Title)
	{
		return _plugin_menuadd(Handle, Title.c_str());
	}

	bool asAddMenuEntry(int Handle, int Entry, std::string &Title)
	{
		return _plugin_menuaddentry(Handle, Entry, Title.c_str());
	}

	bool asAddMenuSeparator(int Handle)
	{
		return _plugin_menuaddseparator(Handle);
	}

	bool asClearMenu(int Handle)
	{
		return _plugin_menuclear(Handle);
	}
}
}