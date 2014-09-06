#pragma once

namespace Script
{
namespace Dbg
{
	void Init(asIScriptEngine *Engine);

	// AngelScript functions
	ULONG_PTR asValFromString(std::string &Value);

	bool asMemWrite(PVOID Address, PVOID Buffer, asUINT Size, asUINT *BytesWritten);
	bool asMemRead(PVOID Address, PVOID Buffer, asUINT Size, asUINT *BytesRead);
	bool asMemFill(PVOID Address, asBYTE Value, asUINT Size);

	ULONG_PTR asMemGetPageSize(ULONG_PTR Base);
	ULONG_PTR asMemGetBaseAddr(ULONG_PTR Address, asUINT *Size);

	bool asCmdExec(std::string &Cmd);
	bool asCmdExecDirect(std::string &Cmd);

	bool asIsValidExpression(std::string &Expression);
	bool asIsDebugging();
	bool asIsJumpGoingToExecute(ULONG_PTR Address);
}
}