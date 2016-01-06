#include "../stdafx.h"

namespace Script
{
namespace Dbg
{
	void Init(asIScriptEngine *Engine)
	{
		//
		// ENUMERATIONS/STRUCTURES
		//
		Engine->SetDefaultNamespace("");

		RegisterDefaultEnums(Engine);
		RegisterWindowsStructs(Engine);
		RegisterDefaultStructs(Engine);

		// Register all debugger variables and functions under "Dbg"
		Engine->SetDefaultNamespace("Dbg");

		//
		// VARIABLES
		//
		VERIFY(Engine->RegisterGlobalProperty("const int PluginHandle", &pluginHandle));

		//
		// FUNCTIONS
		//
#define RegisterFunction(func, def) VERIFY(Engine->RegisterGlobalFunction(def, asFUNCTION(func), asCALL_CDECL));

		RegisterFunction(asDbgInit,              "string DbgInit()");
		RegisterFunction(asDbgExit,              "void DbgExit()");
		RegisterFunction(asMemWrite,             "bool MemWrite(ptr Address, ptr Buffer, uint Size, uint &out BytesWritten = 0)");
		RegisterFunction(asMemRead,              "bool MemRead(ptr Address, ptr Buffer, uint Size, uint &out BytesRead = 0)");
		RegisterFunction(asMemFill,              "bool MemFill(ptr Address, byte Value, uint Size)");
		RegisterFunction(asMemGetPageSize,       "ptr MemGetPageSize(ptr Base)");
		RegisterFunction(asMemGetBaseAddr,       "ptr MemGetBaseAddr(ptr Address, uint &out Size = 0)");
		RegisterFunction(asCmdExec,              "bool CmdExec(string &in Cmd)");
		RegisterFunction(asCmdExecDirect,        "bool CmdExecDirect(string &in Cmd)");
		RegisterFunction(asMemMap,               "bool MemMap(MEMMAP &out Map)");
		RegisterFunction(asIsValidExpression,    "bool IsValidExpression(string &in Expression)");
		RegisterFunction(asIsDebugging,          "bool IsDebugging()");
		RegisterFunction(asIsJumpGoingToExecute, "bool IsJumpGoingToExecute(ptr Address)");
		RegisterFunction(asGetLabelAt,           "bool GetLabelAt(ptr Address, SEGMENTREG Segment, string &out Text)");
		RegisterFunction(asSetLabelAt,           "bool SetLabelAt(ptr Address, string &in Text)");
		RegisterFunction(asGetBookmarkAt,        "bool GetBookmarkAt(ptr Address)");
		RegisterFunction(asSetBookmarkAt,        "bool SetBookmarkAt(ptr Address, bool Enable)");
		RegisterFunction(asGetModuleAt,          "bool GetModuleAt(ptr Address, string &out Module)");
		RegisterFunction(asGetBpxTypeAt,         "BPXTYPE GetBpxTypeAt(ptr Address)");
		RegisterFunction(asValFromString,        "ptr ValFromString(string &in Value)");
		//RegisterFunction(asGetRegDump,           "bool GetRegDump(REGDUMP &out Dump)");
		// DbgValToString
		RegisterFunction(asMemIsValidReadPtr,    "bool MemIsValidReadPtr(ptr Address)");
		RegisterFunction(asGetBpList,            "int GetBpList(BPXTYPE Type, BPMAP &out Map)");
		RegisterFunction(asGetFunctionTypeAt,    "FUNCTYPE GetFunctionTypeAt(ptr Address)");
		RegisterFunction(asGetLoopTypeAt,        "LOOPTYPE GetLoopTypeAt(ptr Address, int Depth)");
		RegisterFunction(asGetBranchDestination, "ptr GetBranchDestination(ptr Address)");
		RegisterFunction(asScriptLoad,           "void ScriptLoad(string &in File)");
		RegisterFunction(asScriptUnload,         "void ScriptUnload()");
		RegisterFunction(asScriptRun,            "void ScriptRun(int DestLine)");
		RegisterFunction(asScriptStep,           "void ScriptStep()");
		RegisterFunction(asScriptBpToggle,       "bool ScriptBpToggle(int Line)");
		RegisterFunction(asScriptBpGet,          "bool ScriptBpGet(int Line)");
		RegisterFunction(asScriptCmdExec,        "bool ScriptCmdExec(string &in Command)");
		RegisterFunction(asScriptAbort,          "void ScriptAbort()");
		RegisterFunction(asScriptGetLineType,    "SCRIPTLINETYPE ScriptGetLineType(int Line)");
		RegisterFunction(asScriptSetIp,          "void ScriptSetIp(int Line)");
		RegisterFunction(asScriptGetBranchInfo,  "bool ScriptGetBranchInfo(int Line, SCRIPTBRANCH &out Info)");
		// DbgSymbolEnum
		RegisterFunction(asAssembleAt,           "bool AssembleAt(ptr Address, string &in Instruction)");
		RegisterFunction(asModBaseFromName,      "ptr ModBaseFromName(string &in Name)");


		RegisterFunction(asByteAt,               "byte byteAt(ptr Address)");
		RegisterFunction(asWordAt,               "word wordAt(ptr Address)");
		RegisterFunction(asDwordAt,              "dword dwordAt(ptr Address)");
		RegisterFunction(asQwordAt,              "qword qwordAt(ptr Address)");
	}

	std::string asDbgInit()
	{
		return std::string(DbgInit());
	}

	void asDbgExit()
	{
		DbgExit();
	}

	bool asMemWrite(PVOID Address, PVOID Buffer, asUINT Size, asUINT *BytesWritten)
	{
		if (DbgMemWrite((duint)Address, (PUCHAR)Buffer, Size))
		{
			if (BytesWritten)
				*BytesWritten = Size;

			return true;
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
		asBYTE buffer[512];
		memset(buffer, Value, min(Size, sizeof(buffer)));

		for (; Size > 0;)
		{
			if (!asMemWrite(Address, buffer, min(Size, sizeof(buffer)), nullptr))
				return false;

			Size -= min(Size, sizeof(buffer));
		}

		return true;
	}

	ULONG_PTR asMemGetPageSize(ULONG_PTR Base)
	{
		return DbgMemGetPageSize(Base);
	}

	ULONG_PTR asMemGetBaseAddr(ULONG_PTR Address, asUINT *Size)
	{
		duint retSize	= 0;
		ULONG_PTR base	= DbgMemFindBaseAddr(Address, &retSize);

		if (*Size)
			*Size = (asUINT)retSize;

		return base;
	}

	bool asCmdExec(std::string &Cmd)
	{
		return DbgCmdExec(Cmd.c_str());
	}

	bool asCmdExecDirect(std::string &Cmd)
	{
		return DbgCmdExecDirect(Cmd.c_str());
	}

	bool asMemMap(MEMMAP *Map)
	{
		if (!Map)
			return false;

		return DbgMemMap(Map);
	}

	bool asIsValidExpression(std::string &Expression)
	{
		return DbgIsValidExpression(Expression.c_str());
	}

	bool asIsDebugging()
	{
		return DbgIsDebugging();
	}

	bool asIsJumpGoingToExecute(ULONG_PTR Address)
	{
		return DbgIsJumpGoingToExecute(Address);
	}

	bool asGetLabelAt(ULONG_PTR Address, SEGMENTREG Segment, std::string *Text)
	{
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));

		if (Text && DbgGetLabelAt(Address, Segment, buffer))
		{
			*Text = std::string(buffer);
			return true;
		}

		return false;
	}

	bool asSetLabelAt(ULONG_PTR Address, std::string &Text)
	{
		return DbgSetLabelAt(Address, Text.c_str());
	}

	bool asGetBookmarkAt(ULONG_PTR Address)
	{
		return DbgGetBookmarkAt(Address);
	}

	bool asSetBookmarkAt(ULONG_PTR Address, bool Enable)
	{
		return DbgSetBookmarkAt(Address, Enable);
	}

	bool asGetModuleAt(ULONG_PTR Address, std::string *Module)
	{
		if (!Module)
			return false;

		char buf[1024];
		if (!DbgGetModuleAt(Address, buf))
			return false;

		*Module = std::string(buf);
		return true;
	}

	BPXTYPE asGetBpxTypeAt(ULONG_PTR Address)
	{
		return DbgGetBpxTypeAt(Address);
	}

	ULONG_PTR asValFromString(std::string &Value)
	{
		return DbgValFromString(Value.c_str());
	}

	bool asGetRegDump(REGDUMP *Dump)
	{
		if (!Dump)
			return false;

		return DbgGetRegDump(Dump);
	}

	bool asMemIsValidReadPtr(ULONG_PTR Address)
	{
		return DbgMemIsValidReadPtr(Address);
	}

	int asGetBpList(BPXTYPE Type, BPMAP *Map)
	{
		if (!Map)
			return 0;

		return DbgGetBpList(Type, Map);
	}

	FUNCTYPE asGetFunctionTypeAt(ULONG_PTR Address)
	{
		return DbgGetFunctionTypeAt(Address);
	}

	LOOPTYPE asGetLoopTypeAt(ULONG_PTR Address, int Depth)
	{
		return DbgGetLoopTypeAt(Address, Depth);
	}

	ULONG_PTR asGetBranchDestination(ULONG_PTR Address)
	{
		return DbgGetBranchDestination(Address);
	}

	void asScriptLoad(std::string &File)
	{
		DbgScriptLoad(File.c_str());
	}

	void asScriptUnload()
	{
		DbgScriptUnload();
	}

	void asScriptRun(int DestLine)
	{
		DbgScriptRun(DestLine);
	}

	void asScriptStep()
	{
		DbgScriptStep();
	}

	bool asScriptBpToggle(int Line)
	{
		return DbgScriptBpToggle(Line);
	}

	bool asScriptBpGet(int Line)
	{
		return DbgScriptBpGet(Line);
	}

	bool asScriptCmdExec(std::string &Command)
	{
		return DbgScriptCmdExec(Command.c_str());
	}

	void asScriptAbort()
	{
		DbgScriptAbort();
	}

	SCRIPTLINETYPE asScriptGetLineType(int Line)
	{
		return DbgScriptGetLineType(Line);
	}

	void asScriptSetIp(int Line)
	{
		DbgScriptSetIp(Line);
	}

	bool asScriptGetBranchInfo(int Line, SCRIPTBRANCH *Info)
	{
		if (!Info)
			return false;

		return DbgScriptGetBranchInfo(Line, Info);
	}

	bool asAssembleAt(ULONG_PTR Address, std::string &Instruction)
	{
		return DbgAssembleAt(Address, Instruction.c_str());
	}

	ULONG_PTR asModBaseFromName(std::string &Name)
	{
		return DbgModBaseFromName(Name.c_str());
	}

	asBYTE asByteAt(ULONG_PTR Address)
	{
		asBYTE val = 0;
		asMemRead((PVOID)Address, &val, sizeof(asBYTE), nullptr);

		return val;
	}

	asWORD asWordAt(ULONG_PTR Address)
	{
		asWORD val = 0;
		asMemRead((PVOID)Address, &val, sizeof(asWORD), nullptr);

		return val;
	}

	asDWORD asDwordAt(ULONG_PTR Address)
	{
		asDWORD val = 0;
		asMemRead((PVOID)Address, &val, sizeof(asDWORD), nullptr);

		return val;
	}

	asQWORD asQwordAt(ULONG_PTR Address)
	{
		asQWORD val = 0;
		asMemRead((PVOID)Address, &val, sizeof(asQWORD), nullptr);

		return val;
	}
}
}