#pragma once

namespace Script
{
#include <string>
#include <assert.h>

#ifdef _DEBUG
#define VERIFY(x) assert((x) >= 0)
#else
#define VERIFY(x) x
#endif

#ifdef _WIN64
#define SetReturnPointerVal(x) SetReturnQWord((asQWORD)(x))
#define SetArgPointerVal SetArgQWord
#else
#define SetReturnPointerVal(x) SetReturnDWord((asDWORD)(x))
#define SetArgPointerVal SetArgDWord
#endif

	struct ModuleDef
	{
		asIScriptModule *asModule;

		asIScriptFunction *asOnPluginSetup;
		asIScriptFunction *asOnInitDebug;
		asIScriptFunction *asOnStopDebug;
		asIScriptFunction *asOnCreateProcess;
		asIScriptFunction *asOnExitProcess;
		asIScriptFunction *asOnCreateThread;
		asIScriptFunction *asOnExitThread;
		asIScriptFunction *asOnSystemBreakpoint;
		asIScriptFunction *asOnLoadDll;
		asIScriptFunction *asOnUnloadDll;
		asIScriptFunction *asOnOutputDebugString;
		asIScriptFunction *asOnException;
		asIScriptFunction *asOnBreakpoint;
		asIScriptFunction *asOnPauseDebug;
		asIScriptFunction *asOnResumeDebug;
		asIScriptFunction *asOnStepped;
		asIScriptFunction *asOnAttach;
		asIScriptFunction *asOnDetach;
		asIScriptFunction *asOnDebugEvent;
		asIScriptFunction *asOnMenuEvent;
	};

	extern asIScriptEngine *EngInst;

	bool EngineInit();
	bool EngineLoad(ModuleDef *Module, const char *Path);
	void EngineMessage(const asSMessageInfo *msg, void *param);

	asIScriptContext *CreateContext(asIScriptFunction *Function);
}