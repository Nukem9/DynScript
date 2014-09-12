#pragma once

namespace Script
{
namespace Dbg
{
	void Init(asIScriptEngine *Engine);

	// AngelScript functions
	std::string asDbgInit();
	void asDbgExit();

	bool asMemWrite(PVOID Address, PVOID Buffer, asUINT Size, asUINT *BytesWritten);
	bool asMemRead(PVOID Address, PVOID Buffer, asUINT Size, asUINT *BytesRead);
	bool asMemFill(PVOID Address, asBYTE Value, asUINT Size);

	ULONG_PTR asMemGetPageSize(ULONG_PTR Base);
	ULONG_PTR asMemGetBaseAddr(ULONG_PTR Address, asUINT *Size);

	bool asCmdExec(std::string &Cmd);
	bool asCmdExecDirect(std::string &Cmd);

	bool asMemMap(MEMMAP *Map);

	bool asIsValidExpression(std::string &Expression);
	bool asIsDebugging();
	bool asIsJumpGoingToExecute(ULONG_PTR Address);

	bool asGetLabelAt(ULONG_PTR Address, SEGMENTREG Segment, std::string *Text);
	bool asSetLabelAt(ULONG_PTR Address, std::string &Text);

	bool asGetBookmarkAt(ULONG_PTR Address);
	bool asSetBookmarkAt(ULONG_PTR Address, bool Enable);

	bool asGetModuleAt(ULONG_PTR Address, std::string *Module);
	BPXTYPE asGetBpxTypeAt(ULONG_PTR Address);

	ULONG_PTR asValFromString(std::string &Value);
	bool asGetRegDump(REGDUMP *Dump);

	bool asMemIsValidReadPtr(ULONG_PTR Address);
	int asGetBpList(BPXTYPE Type, BPMAP *Map);
	FUNCTYPE asGetFunctionTypeAt(ULONG_PTR Address);
	LOOPTYPE asGetLoopTypeAt(ULONG_PTR Address, int Depth);
	ULONG_PTR asGetBranchDestination(ULONG_PTR Address);

	void asScriptLoad(std::string &File);
	void asScriptUnload();
	void asScriptRun(int DestLine);
	void asScriptStep();
	bool asScriptBpToggle(int Line);
	bool asScriptBpGet(int Line);
	bool asScriptCmdExec(std::string &Command);
	void asScriptAbort();
	SCRIPTLINETYPE asScriptGetLineType(int Line);
	void asScriptSetIp(int Line);
	bool asScriptGetBranchInfo(int Line, SCRIPTBRANCH *Info);

	asBYTE asByteAt(ULONG_PTR Address);
	asWORD asWordAt(ULONG_PTR Address);
	asDWORD asDwordAt(ULONG_PTR Address);
	asQWORD asQwordAt(ULONG_PTR Address);
}
}