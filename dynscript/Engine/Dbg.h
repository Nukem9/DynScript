#pragma once

namespace Script
{
namespace Dbg
{
	void Init(asIScriptEngine *Engine);

	// AngelScript functions
	PVOID asValFromString(std::string &Value);

	bool asMemWrite(PVOID Address, PVOID Buffer, asUINT Size, asUINT *BytesWritten);
	bool asMemRead(PVOID Address, PVOID Buffer, asUINT Size, asUINT *BytesRead);
	bool asMemFill(PVOID Address, asBYTE Value, asUINT Size);
}
}