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

		AS_BEGIN_ENUM(SEGMENTREG)
			AS_ADD_ENUM(SEG_DEFAULT)
			AS_ADD_ENUM(SEG_ES)
			AS_ADD_ENUM(SEG_DS)
			AS_ADD_ENUM(SEG_FS)
			AS_ADD_ENUM(SEG_GS)
			AS_ADD_ENUM(SEG_CS)
			AS_ADD_ENUM(SEG_SS)
		AS_END_ENUM()

		AS_BEGIN_ENUM(BPXTYPE)
			AS_ADD_ENUM(bp_none)
			AS_ADD_ENUM(bp_normal)
			AS_ADD_ENUM(bp_hardware)
			AS_ADD_ENUM(bp_memory)
		AS_END_ENUM()

		// Register all debugger variables and functions under "Dbg"
		Engine->SetDefaultNamespace("Dbg");

		//
		// VARIABLES
		//
		VERIFY(Engine->RegisterGlobalProperty("const int PluginHandle", &pluginHandle));

		//
		// FUNCTIONS
		//
		VERIFY(Engine->RegisterGlobalFunction("ptr ValFromString(string &in Value)", asFUNCTION(asValFromString), asCALL_CDECL));

		VERIFY(Engine->RegisterGlobalFunction("bool MemWrite(ptr Address, ptr Buffer, uint Size, uint &out BytesWritten = 0)", asFUNCTION(asMemWrite), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool MemRead(ptr Address, ptr Buffer, uint Size, uint &out BytesRead = 0)", asFUNCTION(asMemRead), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool MemFill(ptr Address, byte Value, uint Size)", asFUNCTION(asMemFill), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("ptr MemGetPageSize(ptr Base)", asFUNCTION(asMemGetPageSize), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("ptr MemGetBaseAddr(ptr Address, uint &out Size = 0)", asFUNCTION(asMemGetBaseAddr), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool CmdExec(string &in Cmd)", asFUNCTION(asCmdExec), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool CmdExecDirect(string &in Cmd)", asFUNCTION(asCmdExecDirect), asCALL_CDECL));
		//VERIFY(Engine->RegisterGlobalFunction("bool MemMap(........)", asFUNCTION(asMemMap), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool IsValidExpression(string &in Expression)", asFUNCTION(asIsValidExpression), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool IsDebugging()", asFUNCTION(asIsDebugging), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool IsJumpGoingToExecute(ptr Address)", asFUNCTION(asIsJumpGoingToExecute), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool GetLabelAt(ptr Address, SEGMENTREG Segment, string &out Text)", asFUNCTION(asGetLabelAt), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("bool SetLabelAt(ptr Address, string &in Text)", asFUNCTION(asSetLabelAt), asCALL_CDECL));

		VERIFY(Engine->RegisterGlobalFunction("byte byteAt(ptr Address)", asFUNCTION(asByteAt), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("word wordAt(ptr Address)", asFUNCTION(asWordAt), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("dword dwordAt(ptr Address)", asFUNCTION(asDwordAt), asCALL_CDECL));
		VERIFY(Engine->RegisterGlobalFunction("qword qwordAt(ptr Address)", asFUNCTION(asQwordAt), asCALL_CDECL));
	}

	ULONG_PTR asValFromString(std::string &Value)
	{
		return DbgValFromString(Value.c_str());
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
			*Size = retSize;

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

// 	bool asMemMap(/*....*/)
// 	{
// 		return false;
// 	}

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