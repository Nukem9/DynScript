#pragma once

namespace Script
{
	void FORCEINLINE RegisterDefaultStructs(asIScriptEngine *Engine)
	{
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
			AS_ADD_STRUCT(ptr, cax)
			AS_ADD_STRUCT(ptr, ccx)
			AS_ADD_STRUCT(ptr, cdx)
			AS_ADD_STRUCT(ptr, cbx)
			AS_ADD_STRUCT(ptr, csp)
			AS_ADD_STRUCT(ptr, cbp)
			AS_ADD_STRUCT(ptr, csi)
			AS_ADD_STRUCT(ptr, cdi)
#ifdef _WIN64
			AS_ADD_STRUCT(ptr, r8)
			AS_ADD_STRUCT(ptr, r9)
			AS_ADD_STRUCT(ptr, r10)
			AS_ADD_STRUCT(ptr, r11)
			AS_ADD_STRUCT(ptr, r12)
			AS_ADD_STRUCT(ptr, r13)
			AS_ADD_STRUCT(ptr, r14)
			AS_ADD_STRUCT(ptr, r15)
#endif
			AS_ADD_STRUCT(ptr, cip)
			AS_ADD_STRUCT(uint, eflags)
			AS_ADD_STRUCT(FLAGS, flags)
			AS_ADD_STRUCT(word, gs)
			AS_ADD_STRUCT(word, fs)
			AS_ADD_STRUCT(word, es)
			AS_ADD_STRUCT(word, ds)
			AS_ADD_STRUCT(word, cs)
			AS_ADD_STRUCT(word, ss)
			AS_ADD_STRUCT(ptr, dr0)
			AS_ADD_STRUCT(ptr, dr1)
			AS_ADD_STRUCT(ptr, dr2)
			AS_ADD_STRUCT(ptr, dr3)
			AS_ADD_STRUCT(ptr, dr6)
			AS_ADD_STRUCT(ptr, dr7)
		AS_END_STRUCT()
	}
}