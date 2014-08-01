#include "../stdafx.h"

namespace Script
{
namespace Dbg
{
	void Init(asIScriptEngine *Engine)
	{
		// Register all debugger variables and functions
		Engine->SetDefaultNamespace("Dbg");

		//
		// VARIABLES
		//
		VERIFY(Engine->RegisterGlobalProperty("const int PluginHandle", &pluginHandle));
		VERIFY(Engine->RegisterGlobalProperty("const handle ProcessHandle", &pluginHandle));

		//
		// FUNCTIONS
		//
		VERIFY(Engine->RegisterGlobalFunction("ptr ValFromString(string &in Value)", asFUNCTION(asValFromString), asCALL_CDECL));

		VERIFY(Engine->RegisterGlobalFunction("bool MemWrite(ptr Address, ptr Buffer, uint Size, uint &out BytesWritten = 0)", asFUNCTION(asMemWrite), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool MemRead(ptr Address, ptr Buffer, uint Size, uint &out BytesRead = 0)", asFUNCTION(asMemRead), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool MemFill(ptr Address, byte Value, uint Size)", asFUNCTION(asMemFill), asCALL_CDECL));
	}

	PVOID asValFromString(std::string &Value)
	{
		return (PVOID)DbgValFromString(Value.c_str());
	}

	bool asMemWrite(PVOID Address, PVOID Buffer, asUINT Size, asUINT *BytesWritten)
	{
		if (DbgMemWrite((duint)Address, (PUCHAR)Buffer, Size))
		{
			if (BytesWritten)
				*BytesWritten = Size;

			return false;
		}

		if (BytesWritten)
			*BytesWritten = 0;

		return false;
	}

	bool asMemRead(PVOID Address, PVOID Buffer, asUINT Size, asUINT *BytesRead)
	{
		if (DbgMemRead((duint)Address, (PUCHAR)Buffer, Size))
		{
			if (BytesRead)
				*BytesRead = Size;

			return true;
		}

		if (BytesRead)
			*BytesRead = 0;

		return false;
	}

	bool asMemFill(PVOID Address, asBYTE Value, asUINT Size)
	{
		asBYTE buffer[256];
		memset(buffer, Value, min(Size, sizeof(buffer)));

		for (;;)
		{
			if (Size <= 0)
				break;

			if (!asMemWrite(Address, buffer, min(Size, sizeof(buffer)), nullptr))
				return false;

			Size -= min(Size, sizeof(buffer));
		}

		return true;
	}
}
}