#pragma once

namespace Script
{
	void FORCEINLINE RegisterDefaultStructs(asIScriptEngine *Engine)
	{
		using namespace std;

		// MEMPAGE
		AS_BEGIN_STRUCT(MEMPAGE)
			AS_STRUCT_ADD(MEMORY_BASIC_INFORMATION, mbi)
			AS_STRUCT_ACCESS(string,                info, STR_GET(info), STR_SET(info))
		AS_END_STRUCT()

		// MEMMAP
		AS_BEGIN_STRUCT(MEMMAP)
			AS_STRUCT_ADD(int,            count)
			AS_STRUCT_ADD_ARRAY(MEMPAGE&, page,
			-> MEMPAGE*
			{
				if (Index >= (asUINT)Obj->count)
					return nullptr;

				return &Obj->page[Index];
			})
		AS_END_STRUCT()

		// BRIDGEBP
		AS_BEGIN_STRUCT(BRIDGEBP)
			AS_STRUCT_ADD(BPXTYPE,   type)
			AS_STRUCT_ADD(ptr,       addr)
			AS_STRUCT_ADD(bool,      enabled)
			AS_STRUCT_ADD(bool,      singleshoot)
			AS_STRUCT_ADD(bool,      active)
			AS_STRUCT_ACCESS(string, name, STR_GET(name), STR_SET(name))
			AS_STRUCT_ACCESS(string, mod,  STR_GET(mod),  STR_SET(mod))
			AS_STRUCT_ADD(word,      slot)
		AS_END_STRUCT()

		// BPMAP
		AS_BEGIN_STRUCT(BPMAP)
			AS_STRUCT_ADD(int,          count)
			AS_STRUCT_ADD_ARRAY(BPMAP&, bp,
			-> BRIDGEBP*
			{
				if (Index >= (asUINT)Obj->count)
					return nullptr;

				return &Obj->bp[Index];
			})
		AS_END_STRUCT()

		// FUNCTION
		AS_BEGIN_STRUCT(FUNCTION)
			AS_STRUCT_ADD(ptr, start)
			AS_STRUCT_ADD(ptr, end)
		AS_END_STRUCT()

		// LOOP
		AS_BEGIN_STRUCT(LOOP)
			AS_STRUCT_ADD(int, depth)
			AS_STRUCT_ADD(ptr, start)
			AS_STRUCT_ADD(ptr, end)
		AS_END_STRUCT()

		// ADDRINFO
		AS_BEGIN_STRUCT(ADDRINFO)
			AS_STRUCT_ADD(int,       flags)
			AS_STRUCT_ACCESS(string, module,  STR_GET(module),  STR_SET(module))
			AS_STRUCT_ACCESS(string, label,   STR_GET(label),   STR_SET(label))
			AS_STRUCT_ACCESS(string, comment, STR_GET(comment), STR_SET(comment))
			AS_STRUCT_ADD(bool,      isbookmark)
			AS_STRUCT_ADD(FUNCTION,  function)
			AS_STRUCT_ADD(LOOP,      loop)
		AS_END_STRUCT()

		// SYMBOLINFO
		AS_BEGIN_STRUCT(SYMBOLINFO)
			AS_STRUCT_ADD(ptr,       addr)
			AS_STRUCT_ACCESS(string, decoratedSymbol,   STR_GET(decoratedSymbol),   {  })
			AS_STRUCT_ACCESS(string, undecoratedSymbol, STR_GET(undecoratedSymbol), {  })
		AS_END_STRUCT()

		// SYMBOLMODULEINFO
		AS_BEGIN_STRUCT(SYMBOLMODULEINFO)
			AS_STRUCT_ADD(ptr,       base)
			AS_STRUCT_ACCESS(string, name, STR_GET(name), STR_SET(name))
		AS_END_STRUCT()

#if 0
		// SYMBOLCBINFO !!TODO!!
		AS_BEGIN_STRUCT(SYMBOLCBINFO)
			AS_STRUCT_ADD(ptr, base)
			AS_STRUCT_ADD(ptr, cbSymbolEnum)
			AS_STRUCT_ADD(ptr, user)
		AS_END_STRUCT()
#endif

		// FLAGS
		AS_BEGIN_STRUCT(FLAGS)
			AS_STRUCT_ADD(bool, c)
			AS_STRUCT_ADD(bool, p)
			AS_STRUCT_ADD(bool, a)
			AS_STRUCT_ADD(bool, z)
			AS_STRUCT_ADD(bool, s)
			AS_STRUCT_ADD(bool, t)
			AS_STRUCT_ADD(bool, i)
			AS_STRUCT_ADD(bool, d)
			AS_STRUCT_ADD(bool, o)
		AS_END_STRUCT()

		// REGDUMP
		AS_BEGIN_STRUCT(REGDUMP)
			AS_STRUCT_ADD(ptr,   cax)
			AS_STRUCT_ADD(ptr,   ccx)
			AS_STRUCT_ADD(ptr,   cdx)
			AS_STRUCT_ADD(ptr,   cbx)
			AS_STRUCT_ADD(ptr,   csp)
			AS_STRUCT_ADD(ptr,   cbp)
			AS_STRUCT_ADD(ptr,   csi)
			AS_STRUCT_ADD(ptr,   cdi)
#ifdef _WIN64
			AS_STRUCT_ADD(ptr,   r8)
			AS_STRUCT_ADD(ptr,   r9)
			AS_STRUCT_ADD(ptr,   r10)
			AS_STRUCT_ADD(ptr,   r11)
			AS_STRUCT_ADD(ptr,   r12)
			AS_STRUCT_ADD(ptr,   r13)
			AS_STRUCT_ADD(ptr,   r14)
			AS_STRUCT_ADD(ptr,   r15)
#endif
			AS_STRUCT_ADD(ptr,   cip)
			AS_STRUCT_ADD(uint,  eflags)
			AS_STRUCT_ADD(FLAGS, flags)
			AS_STRUCT_ADD(word,  gs)
			AS_STRUCT_ADD(word,  fs)
			AS_STRUCT_ADD(word,  es)
			AS_STRUCT_ADD(word,  ds)
			AS_STRUCT_ADD(word,  cs)
			AS_STRUCT_ADD(word,  ss)
			AS_STRUCT_ADD(ptr,   dr0)
			AS_STRUCT_ADD(ptr,   dr1)
			AS_STRUCT_ADD(ptr,   dr2)
			AS_STRUCT_ADD(ptr,   dr3)
			AS_STRUCT_ADD(ptr,   dr6)
			AS_STRUCT_ADD(ptr,   dr7)
		AS_END_STRUCT()

		// DISASM_ARG
		AS_BEGIN_STRUCT(DISASM_ARG)
			AS_STRUCT_ADD(DISASM_ARGTYPE, type)
			AS_STRUCT_ADD(SEGMENTREG, segment)
			AS_STRUCT_ACCESS(string,  mnemonic, STR_GET(mnemonic), STR_SET(mnemonic))
			AS_STRUCT_ADD(ptr,        constant)
			AS_STRUCT_ADD(ptr,        value)
			AS_STRUCT_ADD(ptr,        memvalue)
		AS_END_STRUCT()

		// DISASM_INSTR
		AS_BEGIN_STRUCT(DISASM_INSTR)
			AS_STRUCT_ACCESS(string,         instruction, STR_GET(instruction), STR_SET(instruction))
			AS_STRUCT_ADD(DISASM_INSTRTYPE,  type)
			AS_STRUCT_ADD(int,               argcount)
			AS_STRUCT_ADD(int,               instr_size)
			AS_STRUCT_ADD_ARRAY(DISASM_ARG&, arg,
			-> DISASM_ARG*
			{
				if (Index >= ARRAYSIZE(Obj->arg))
					return nullptr;

				return &Obj->arg[Index];
			})
		AS_END_STRUCT()

		// STACK_COMMENT
		AS_BEGIN_STRUCT(STACK_COMMENT)
			AS_STRUCT_ADD_ARRAY(byte, color,
			-> BYTE
			{
				if (Index >= ARRAYSIZE(Obj->color))
					return 0;

				return Obj->color[Index];
			})
			AS_STRUCT_ACCESS(string, comment, STR_GET(comment), STR_SET(comment))
		AS_END_STRUCT()

		// THREADINFO
		AS_BEGIN_STRUCT(THREADINFO)
			AS_STRUCT_ADD(int,       ThreadNumber)
			AS_STRUCT_ADD(handle,    hThread)
			AS_STRUCT_ADD(dword,     dwThreadId)
			AS_STRUCT_ADD(ptr,       ThreadStartAddress)
			AS_STRUCT_ADD(ptr,       ThreadLocalBase)
			AS_STRUCT_ACCESS(string, threadName, STR_GET(threadName), STR_SET(threadName))
		AS_END_STRUCT()

		// THREADALLINFO
		AS_BEGIN_STRUCT(THREADALLINFO)
			AS_STRUCT_ADD(THREADINFO,       BasicInfo)
			AS_STRUCT_ADD(ptr,              ThreadCip)
			AS_STRUCT_ADD(dword,            SuspendCount)
			AS_STRUCT_ADD(THREADPRIORITY,   Priority)
			AS_STRUCT_ADD(THREADWAITREASON, WaitReason)
			AS_STRUCT_ADD(dword,            LastError)
		AS_END_STRUCT()

		// THREADLIST
		AS_BEGIN_STRUCT(THREADLIST)
			AS_STRUCT_ADD(int,                  count)
			AS_STRUCT_ADD_ARRAY(THREADALLINFO&, list,
			-> THREADALLINFO*
				{
					if (Index >= (asUINT)Obj->count)
						return 0;

					return &Obj->list[Index];
				})
			AS_STRUCT_ADD(int,                  CurrentThread)
		AS_END_STRUCT()

		// MEMORY_INFO
		AS_BEGIN_STRUCT(MEMORY_INFO)
			AS_STRUCT_ADD(ptr,         value)
			AS_STRUCT_ADD(MEMORY_SIZE, size)
			AS_STRUCT_ACCESS(string,   mnemonic, STR_GET(mnemonic), STR_SET(mnemonic))
		AS_END_STRUCT()

		// VALUE_INFO
		AS_BEGIN_STRUCT(VALUE_INFO)
			AS_STRUCT_ADD(ptr,         value)
			AS_STRUCT_ADD(MEMORY_SIZE, size)
		AS_END_STRUCT()

		// BASIC_INSTRUCTION_INFO
		AS_BEGIN_STRUCT(BASIC_INSTRUCTION_INFO)
			AS_STRUCT_ADD(dword,       type)
			AS_STRUCT_ADD(VALUE_INFO,  value)
			AS_STRUCT_ADD(MEMORY_INFO, memory)
			AS_STRUCT_ADD(ptr,         addr)
			AS_STRUCT_ADD(bool,        branch)
			AS_STRUCT_ADD(bool,        call)
		AS_END_STRUCT()

		// SCRIPTBRANCH
		AS_BEGIN_STRUCT(SCRIPTBRANCH)
			AS_STRUCT_ADD(SCRIPTBRANCHTYPE, type)
			AS_STRUCT_ADD(int,              dest)
			AS_STRUCT_ACCESS(string,        branchlabel, STR_GET(branchlabel), STR_SET(branchlabel))
		AS_END_STRUCT()

		// FUNCTION_LOOP_INFO
		AS_BEGIN_STRUCT(FUNCTION_LOOP_INFO)
			AS_STRUCT_ADD(ptr,  addr)
			AS_STRUCT_ADD(ptr,  start)
			AS_STRUCT_ADD(ptr,  end)
			AS_STRUCT_ADD(bool, manual)
			AS_STRUCT_ADD(int,  depth)
		AS_END_STRUCT()
	}

	void FORCEINLINE RegisterWindowsStructs(asIScriptEngine *Engine)
	{
		using namespace std;

		// GUID
		AS_BEGIN_STRUCT(GUID)
			AS_STRUCT_ADD(dword,     Data1)
			AS_STRUCT_ADD(word,      Data2)
			AS_STRUCT_ADD(word,      Data3)
			AS_STRUCT_ADD_ARRAY(ptr, Data4,
			-> BYTE
			{
				if (Index >= ARRAYSIZE(Obj->Data4))
					return 0;

				return Obj->Data4[Index];
			})
		AS_END_STRUCT()

		// PROCESS_INFORMATION
		AS_BEGIN_STRUCT(PROCESS_INFORMATION)
			AS_STRUCT_ADD(handle, hProcess)
			AS_STRUCT_ADD(handle, hThread)
			AS_STRUCT_ADD(dword,  dwProcessId)
			AS_STRUCT_ADD(dword,  dwThreadId)
		AS_END_STRUCT()

		// MEMORY_BASIC_INFORMATION
		AS_BEGIN_STRUCT(MEMORY_BASIC_INFORMATION)
			AS_STRUCT_ADD(ptr,   BaseAddress)
			AS_STRUCT_ADD(ptr,   AllocationBase)
			AS_STRUCT_ADD(dword, AllocationProtect)
			AS_STRUCT_ADD(ptr,   RegionSize)
			AS_STRUCT_ADD(dword, State)
			AS_STRUCT_ADD(dword, Protect)
			AS_STRUCT_ADD(dword, Type)
		AS_END_STRUCT()

		// EXCEPTION_RECORD
		AS_BEGIN_STRUCT(EXCEPTION_RECORD)
			AS_STRUCT_ADD(dword,     ExceptionCode)
			AS_STRUCT_ADD(dword,     ExceptionFlags)
			AS_STRUCT_ACCESS_MOD(EXCEPTION_RECORD, ExceptionRecord, "&", { return Obj->ExceptionRecord; }, { Obj->ExceptionRecord = &Val; })
			AS_STRUCT_ADD(ptr,       ExceptionAddress)
			AS_STRUCT_ADD(dword,     NumberParameters)
			AS_STRUCT_ADD_ARRAY(ptr, ExceptionInformation,
			-> ULONG_PTR
			{
				if (Index >= ARRAYSIZE(Obj->ExceptionInformation))
					return 0;

				return Obj->ExceptionInformation[Index];
			})
		AS_END_STRUCT()

		// IMAGEHLP_MODULE64
		AS_BEGIN_STRUCT(IMAGEHLP_MODULE64)
			AS_STRUCT_ADD(dword,      SizeOfStruct)
			AS_STRUCT_ADD(qword,      BaseOfImage)
			AS_STRUCT_ADD(dword,      ImageSize)
			AS_STRUCT_ADD(dword,      TimeDateStamp)
			AS_STRUCT_ADD(dword,      CheckSum)
			AS_STRUCT_ADD(dword,      NumSyms)
			// AS_STRUCT_ADD(SYM_TYPE, SymType)
			AS_STRUCT_ACCESS(string,  ModuleName,      STR_GET(ModuleName),      STR_SET(ModuleName))
			AS_STRUCT_ACCESS(string,  ImageName,       STR_GET(ImageName),       STR_SET(ImageName))
			AS_STRUCT_ACCESS(string,  LoadedImageName, STR_GET(LoadedImageName), STR_SET(LoadedImageName))
			AS_STRUCT_ACCESS(string,  LoadedPdbName,   STR_GET(LoadedPdbName),   STR_SET(LoadedPdbName))
			AS_STRUCT_ADD(dword,      CVSig)
			AS_STRUCT_ADD_ARRAY(byte, CVData,
			-> BYTE
			{
				if (Index >= ARRAYSIZE(Obj->CVData))
					return 0;

				return Obj->CVData[Index];
			})
			AS_STRUCT_ADD(dword,      PdbSig)
			AS_STRUCT_ADD(GUID,       PdbSig70)
			AS_STRUCT_ADD(dword,      PdbAge)
			AS_STRUCT_ADD(int,        PdbUnmatched)
			AS_STRUCT_ADD(int,        DbgUnmatched)
			AS_STRUCT_ADD(int,        LineNumbers)
			AS_STRUCT_ADD(int,        GlobalSymbols)
			AS_STRUCT_ADD(int,        TypeInfo)
		AS_END_STRUCT()

		// EXCEPTION_DEBUG_INFO
		AS_BEGIN_STRUCT(EXCEPTION_DEBUG_INFO)
			AS_STRUCT_ADD(EXCEPTION_RECORD, ExceptionRecord)
			AS_STRUCT_ADD(dword,            dwFirstChance)
		AS_END_STRUCT()

		// CREATE_THREAD_DEBUG_INFO
		AS_BEGIN_STRUCT(CREATE_THREAD_DEBUG_INFO)
			AS_STRUCT_ADD(handle, hThread)
			AS_STRUCT_ADD(ptr,    lpThreadLocalBase)
			AS_STRUCT_ADD(ptr,    lpStartAddress)
		AS_END_STRUCT()

		// CREATE_PROCESS_DEBUG_INFO
		AS_BEGIN_STRUCT(CREATE_PROCESS_DEBUG_INFO)
			AS_STRUCT_ADD(handle, hFile)
			AS_STRUCT_ADD(handle, hProcess)
			AS_STRUCT_ADD(handle, hThread)
			AS_STRUCT_ADD(ptr,    lpBaseOfImage)
			AS_STRUCT_ADD(dword,  dwDebugInfoFileOffset)
			AS_STRUCT_ADD(dword,  nDebugInfoSize)
			AS_STRUCT_ADD(ptr,    lpThreadLocalBase)
			AS_STRUCT_ADD(ptr,    lpStartAddress)
			AS_STRUCT_ADD(ptr,    lpImageName)
			AS_STRUCT_ADD(word,   fUnicode)
		AS_END_STRUCT()

		// EXIT_THREAD_DEBUG_INFO
		AS_BEGIN_STRUCT(EXIT_THREAD_DEBUG_INFO)
			AS_STRUCT_ADD(dword, dwExitCode)
		AS_END_STRUCT()

		// EXIT_PROCESS_DEBUG_INFO
		AS_BEGIN_STRUCT(EXIT_PROCESS_DEBUG_INFO)
			AS_STRUCT_ADD(dword, dwExitCode)
		AS_END_STRUCT()

		// LOAD_DLL_DEBUG_INFO
		AS_BEGIN_STRUCT(LOAD_DLL_DEBUG_INFO)
			AS_STRUCT_ADD(handle, hFile)
			AS_STRUCT_ADD(ptr,    lpBaseOfDll)
			AS_STRUCT_ADD(dword,  dwDebugInfoFileOffset)
			AS_STRUCT_ADD(dword,  nDebugInfoSize)
			AS_STRUCT_ADD(ptr,    lpImageName)
			AS_STRUCT_ADD(word,   fUnicode)
		AS_END_STRUCT()

		// UNLOAD_DLL_DEBUG_INFO
		AS_BEGIN_STRUCT(UNLOAD_DLL_DEBUG_INFO)
			AS_STRUCT_ADD(ptr, lpBaseOfDll)
		AS_END_STRUCT()

		// OUTPUT_DEBUG_STRING_INFO
		AS_BEGIN_STRUCT(OUTPUT_DEBUG_STRING_INFO)
			AS_STRUCT_ADD(ptr,  lpDebugStringData)
			AS_STRUCT_ADD(word, fUnicode)
			AS_STRUCT_ADD(word, nDebugStringLength)
		AS_END_STRUCT()

		// RIP_INFO
		AS_BEGIN_STRUCT(RIP_INFO)
			AS_STRUCT_ADD(dword, dwError)
			AS_STRUCT_ADD(dword, dwType)
		AS_END_STRUCT()

		// DEBUG_EVENT
		AS_BEGIN_STRUCT(DEBUG_EVENT)
			AS_STRUCT_ADD(dword, dwDebugEventCode)
			AS_STRUCT_ADD(dword, dwProcessId)
			AS_STRUCT_ADD(dword, dwThreadId)
			VERIFY(Engine->RegisterObjectProperty(__zname, "EXCEPTION_DEBUG_INFO      Exception",         asOFFSET(objDecl, u.Exception)));
			VERIFY(Engine->RegisterObjectProperty(__zname, "CREATE_THREAD_DEBUG_INFO  CreateThread",      asOFFSET(objDecl, u.CreateThread)));
			VERIFY(Engine->RegisterObjectProperty(__zname, "CREATE_PROCESS_DEBUG_INFO CreateProcessInfo", asOFFSET(objDecl, u.CreateProcessInfo)));
			VERIFY(Engine->RegisterObjectProperty(__zname, "EXIT_THREAD_DEBUG_INFO    ExitThread",        asOFFSET(objDecl, u.ExitThread)));
			VERIFY(Engine->RegisterObjectProperty(__zname, "LOAD_DLL_DEBUG_INFO       LoadDll",           asOFFSET(objDecl, u.LoadDll)));
			VERIFY(Engine->RegisterObjectProperty(__zname, "UNLOAD_DLL_DEBUG_INFO     UnloadDll",         asOFFSET(objDecl, u.UnloadDll)));
			VERIFY(Engine->RegisterObjectProperty(__zname, "OUTPUT_DEBUG_STRING_INFO  DebugString",       asOFFSET(objDecl, u.DebugString)));
			VERIFY(Engine->RegisterObjectProperty(__zname, "RIP_INFO                  RipInfo",           asOFFSET(objDecl, u.RipInfo)));
		AS_END_STRUCT()
	}
}