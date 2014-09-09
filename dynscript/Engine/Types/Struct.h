#pragma once

namespace Script
{
	template <typename Function>
	struct function_traits
		: public function_traits < decltype(&Function::operator()) >
	{};

	template <typename ClassType, typename ReturnType, typename... Args>
	struct function_traits < ReturnType(ClassType::*)(Args...) const >
	{
		typedef ReturnType(*pointer)(Args...);
		typedef std::function<ReturnType(Args...)> function;
	};

	template <typename Function>
	typename function_traits<Function>::pointer
	to_function_pointer(Function& lambda)
	{
		return static_cast<typename function_traits<Function>::pointer>(lambda);
	}

	template <typename Function>
	typename function_traits<Function>::function
	to_function(Function& lambda)
	{
		return static_cast<typename function_traits<Function>::function>(lambda);
	}

	void FORCEINLINE RegisterDefaultStructs(asIScriptEngine *Engine)
	{
		using namespace std;

		// MEMPAGE
		AS_BEGIN_STRUCT(MEMPAGE)
			AS_ADD_STRUCT(MEMORY_BASIC_INFORMATION, mbi)
			AS_ADD_STRUCT_ACCESS(string,            info, STR_GET(info), STR_SET(info))
		AS_END_STRUCT()

		// MEMMAP
		AS_BEGIN_STRUCT(MEMMAP)
			AS_ADD_STRUCT(int,            count)
			AS_ADD_STRUCT_ARRAY(MEMPAGE&, page,
			-> MEMPAGE*
			{
				if (Index > (asUINT)Obj->count)
					return nullptr;

				return &Obj->page[Index];
			})
		AS_END_STRUCT()

		// BRIDGEBP
		AS_BEGIN_STRUCT(BRIDGEBP)
			AS_ADD_STRUCT(BPXTYPE,       type)
			AS_ADD_STRUCT(ptr,           addr)
			AS_ADD_STRUCT(bool,          enabled)
			AS_ADD_STRUCT(bool,          singleshoot)
			AS_ADD_STRUCT(bool,          active)
			AS_ADD_STRUCT_ACCESS(string, name, STR_GET(name), STR_SET(name))
			AS_ADD_STRUCT_ACCESS(string, mod,  STR_GET(mod),  STR_SET(mod))
			AS_ADD_STRUCT(word,          slot)
		AS_END_STRUCT()

		// BPMAP
		AS_BEGIN_STRUCT(BPMAP)
			AS_ADD_STRUCT(int,          count)
			AS_ADD_STRUCT_ARRAY(BPMAP&, bp,
			-> BRIDGEBP*
			{
				if (Index > (asUINT)Obj->count)
				return nullptr;

				return &Obj->bp[Index];
			})
		AS_END_STRUCT()

		// FUNCTION
		AS_BEGIN_STRUCT(FUNCTION)
			AS_ADD_STRUCT(ptr, start)
			AS_ADD_STRUCT(ptr, end)
		AS_END_STRUCT()

		// LOOP
		AS_BEGIN_STRUCT(LOOP)
			AS_ADD_STRUCT(int, depth)
			AS_ADD_STRUCT(ptr, start)
			AS_ADD_STRUCT(ptr, end)
		AS_END_STRUCT()

		// ADDRINFO
		AS_BEGIN_STRUCT(ADDRINFO)
			AS_ADD_STRUCT(int,           flags)
			AS_ADD_STRUCT_ACCESS(string, module,  STR_GET(module),  STR_SET(module))
			AS_ADD_STRUCT_ACCESS(string, label,   STR_GET(label),   STR_SET(label))
			AS_ADD_STRUCT_ACCESS(string, comment, STR_GET(comment), STR_SET(comment))
			AS_ADD_STRUCT(bool,          isbookmark)
			AS_ADD_STRUCT(FUNCTION,      function)
			AS_ADD_STRUCT(LOOP,          loop)
		AS_END_STRUCT()

		// SYMBOLINFO
		AS_BEGIN_STRUCT(SYMBOLINFO)
			AS_ADD_STRUCT(ptr,           addr)
			AS_ADD_STRUCT_ACCESS(string, decoratedSymbol,   STR_GET(decoratedSymbol),   { assert(false); })
			AS_ADD_STRUCT_ACCESS(string, undecoratedSymbol, STR_GET(undecoratedSymbol), { assert(false); })
		AS_END_STRUCT()

		// SYMBOLMODULEINFO
		AS_BEGIN_STRUCT(SYMBOLMODULEINFO)
			AS_ADD_STRUCT(ptr,           base)
			AS_ADD_STRUCT_ACCESS(string, name, STR_GET(name), STR_SET(name))
		AS_END_STRUCT()

		// SYMBOLCBINFO !!TODO!!

		// FLAGS
		AS_BEGIN_STRUCT(FLAGS)
			AS_ADD_STRUCT(bool, c)
			AS_ADD_STRUCT(bool, p)
			AS_ADD_STRUCT(bool, a)
			AS_ADD_STRUCT(bool, z)
			AS_ADD_STRUCT(bool, s)
			AS_ADD_STRUCT(bool, t)
			AS_ADD_STRUCT(bool, i)
			AS_ADD_STRUCT(bool, d)
			AS_ADD_STRUCT(bool, o)
		AS_END_STRUCT()

		// REGDUMP
		AS_BEGIN_STRUCT(REGDUMP)
			AS_ADD_STRUCT(ptr,   cax)
			AS_ADD_STRUCT(ptr,   ccx)
			AS_ADD_STRUCT(ptr,   cdx)
			AS_ADD_STRUCT(ptr,   cbx)
			AS_ADD_STRUCT(ptr,   csp)
			AS_ADD_STRUCT(ptr,   cbp)
			AS_ADD_STRUCT(ptr,   csi)
			AS_ADD_STRUCT(ptr,   cdi)
#ifdef _WIN64
			AS_ADD_STRUCT(ptr,   r8)
			AS_ADD_STRUCT(ptr,   r9)
			AS_ADD_STRUCT(ptr,   r10)
			AS_ADD_STRUCT(ptr,   r11)
			AS_ADD_STRUCT(ptr,   r12)
			AS_ADD_STRUCT(ptr,   r13)
			AS_ADD_STRUCT(ptr,   r14)
			AS_ADD_STRUCT(ptr,   r15)
#endif
			AS_ADD_STRUCT(ptr,   cip)
			AS_ADD_STRUCT(uint,  eflags)
			AS_ADD_STRUCT(FLAGS, flags)
			AS_ADD_STRUCT(word,  gs)
			AS_ADD_STRUCT(word,  fs)
			AS_ADD_STRUCT(word,  es)
			AS_ADD_STRUCT(word,  ds)
			AS_ADD_STRUCT(word,  cs)
			AS_ADD_STRUCT(word,  ss)
			AS_ADD_STRUCT(ptr,   dr0)
			AS_ADD_STRUCT(ptr,   dr1)
			AS_ADD_STRUCT(ptr,   dr2)
			AS_ADD_STRUCT(ptr,   dr3)
			AS_ADD_STRUCT(ptr,   dr6)
			AS_ADD_STRUCT(ptr,   dr7)
		AS_END_STRUCT()

		// DISASM_ARG
		AS_BEGIN_STRUCT(DISASM_ARG)
			AS_ADD_STRUCT(DISASM_ARGTYPE, type)
			AS_ADD_STRUCT(SEGMENTREG,     segment)
			AS_ADD_STRUCT_ACCESS(string,  mnemonic, STR_GET(mnemonic), STR_SET(mnemonic))
			AS_ADD_STRUCT(ptr,            constant)
			AS_ADD_STRUCT(ptr,            value)
			AS_ADD_STRUCT(ptr,            memvalue)
		AS_END_STRUCT()

		// DISASM_INSTR
		AS_BEGIN_STRUCT(DISASM_INSTR)
			AS_ADD_STRUCT_ACCESS(string,     instruction, STR_GET(instruction), STR_SET(instruction))
			AS_ADD_STRUCT(DISASM_INSTRTYPE,  type)
			AS_ADD_STRUCT(int,               argcount)
			AS_ADD_STRUCT(int,               instr_size)
			AS_ADD_STRUCT_ARRAY(DISASM_ARG&, arg,
			-> DISASM_ARG*
			{
				if (Index > ARRAYSIZE(Obj->arg))
					return nullptr;

				return &Obj->arg[Index];
			})
		AS_END_STRUCT()
	}

	void FORCEINLINE RegisterWindowsStructs(asIScriptEngine *Engine)
	{
		// MEMORY_BASIC_INFORMATION
		AS_BEGIN_STRUCT(MEMORY_BASIC_INFORMATION)
			AS_ADD_STRUCT(ptr,   BaseAddress)
			AS_ADD_STRUCT(ptr,   AllocationBase)
			AS_ADD_STRUCT(dword, AllocationProtect)
			AS_ADD_STRUCT(ptr,   RegionSize)
			AS_ADD_STRUCT(dword, State)
			AS_ADD_STRUCT(dword, Protect)
			AS_ADD_STRUCT(dword, Type)
		AS_END_STRUCT()

		// EXCEPTION_RECORD
		AS_BEGIN_STRUCT(EXCEPTION_RECORD)
			AS_ADD_STRUCT(dword,     ExceptionCode)
			AS_ADD_STRUCT(dword,     ExceptionFlags)
			AS_ADD_STRUCT_ACCESS_MOD(EXCEPTION_RECORD, ExceptionRecord, "&", { return Obj->ExceptionRecord; }, { Obj->ExceptionRecord = &Val; })
			AS_ADD_STRUCT(ptr,       ExceptionAddress)
			AS_ADD_STRUCT(dword,     NumberParameters)
			AS_ADD_STRUCT_ARRAY(ptr, ExceptionInformation,
			-> ULONG_PTR
			{
				if (Index > ARRAYSIZE(Obj->ExceptionInformation))
					return 0;

				return Obj->ExceptionInformation[Index];
			})
		AS_END_STRUCT()

		// CREATE_THREAD_DEBUG_INFO
		AS_BEGIN_STRUCT(CREATE_THREAD_DEBUG_INFO)
			AS_ADD_STRUCT(handle, hThread)
			AS_ADD_STRUCT(ptr,    lpThreadLocalBase)
			AS_ADD_STRUCT(ptr,    lpStartAddress)
		AS_END_STRUCT()
	}
}