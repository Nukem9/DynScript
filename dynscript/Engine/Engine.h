#pragma once

namespace Script
{
#include <string>
#include <assert.h>

#define VERIFY(x) assert((x) >= 0)

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