#include "stdafx.h"

int pluginHandle;
HWND hwndDlg;
int hMenu;

Script::ModuleDef def;

void InitDebugCallback(CBTYPE Type, PLUG_CB_INITDEBUG *Info)
{
	std::string file(Info->szFileName);

	// void OnInitDebug(string &in File)
	asExecuteDynamic(def.asOnInitDebug, (OBJECT)&file);
}

void StopDebugCallback(CBTYPE Type, PLUG_CB_STOPDEBUG *Info)
{
	// void OnStopDebug()
	asExecuteDynamic(def.asOnStopDebug);
}

void CreateProcessCallback(CBTYPE Type, PLUG_CB_CREATEPROCESS *Info)
{
}

void ExitProcessCallback(CBTYPE Type, PLUG_CB_EXITPROCESS *Info)
{
	// void OnExitProcess(uint ExitCode)
	asExecuteDynamic(def.asOnExitProcess, Info->ExitProcess->dwExitCode);
}

void CreateThreadCallback(CBTYPE Type, PLUG_CB_CREATETHREAD *Info)
{
}

void ExitThreadCallback(CBTYPE Type, PLUG_CB_EXITTHREAD *Info)
{
	// void OnExitThread(uint ThreadId, uint ExitCode)
	asExecuteDynamic(def.asOnExitThread, Info->dwThreadId, Info->ExitThread->dwExitCode);
}

void SystemBreakpointCallback(CBTYPE Type, PLUG_CB_SYSTEMBREAKPOINT *Info)
{
	// void OnSystemBreakpoint()
	asExecuteDynamic(def.asOnSystemBreakpoint);
}

void LoadDllCallback(CBTYPE Type, PLUG_CB_LOADDLL *Info)
{
}

void UnloadDllCallback(CBTYPE Type, PLUG_CB_UNLOADDLL *Info)
{
	// void OnUnloadDll(ptr DllBase)
	asExecuteDynamic(def.asOnUnloadDll, (ULONG_PTR)Info->UnloadDll->lpBaseOfDll);
}

void DebugStringCallback(CBTYPE Type, PLUG_CB_OUTPUTDEBUGSTRING *Info)
{
	//
	// Convert from unicode to multi-byte if needed
	//
	char buffer[2048];
	memset(buffer, 0, sizeof(buffer));

	size_t maxlen = min(ARRAYSIZE(buffer), Info->DebugString->nDebugStringLength);

	if (Info->DebugString->fUnicode)
	{
		wchar_t wcharbuf[2048];

		if (DbgMemRead((duint)Info->DebugString->lpDebugStringData, (PUCHAR)wcharbuf, maxlen * sizeof(wchar_t)))
			wcstombs(buffer, wcharbuf, ARRAYSIZE(buffer));
	}
	else
	{
		DbgMemRead((duint)Info->DebugString->lpDebugStringData, (PUCHAR)buffer, maxlen);
	}

	//
	// Convert to script object
	//
	std::string message(buffer);

	// void OnOutputDebugString(string &in Message)
	asExecuteDynamic(def.asOnOutputDebugString, (OBJECT)&message);
}

void ExceptionCallback(CBTYPE Type, PLUG_CB_EXCEPTION *Info)
{
}

void BreakpointCallback(CBTYPE Type, PLUG_CB_BREAKPOINT *Info)
{
	std::string name(Info->breakpoint->name);
	std::string mod(Info->breakpoint->mod);

	// void OnBreakpoint(BPXTYPE Type, ptr Address, string &in Name, string &in Module)
	asExecuteDynamic(def.asOnBreakpoint, (int)Info->breakpoint->type, Info->breakpoint->addr, (OBJECT)&name, (OBJECT)&mod);
}

void PauseCallback(CBTYPE Type, PLUG_CB_PAUSEDEBUG *Info)
{
	// void OnPauseDebug()
	asExecuteDynamic(def.asOnPauseDebug);
}

void ResumeCallback(CBTYPE Type, PLUG_CB_RESUMEDEBUG *Info)
{
	// void OnResumeDebug()
	asExecuteDynamic(def.asOnResumeDebug);
}

void SteppedCallback(CBTYPE Type, PLUG_CB_STEPPED *Info)
{
	// void OnStepped()
	asExecuteDynamic(def.asOnStepped);
}

void AttachCallback(CBTYPE Type, PLUG_CB_ATTACH *Info)
{
	// void OnAttach(uint ProcessId)
	asExecuteDynamic(def.asOnAttach, Info->dwProcessId);
}

void DetachCallback(CBTYPE Type, PLUG_CB_DETACH *Info)
{
	// void OnDetach(uint ProcessId, uint ThreadId)
	asExecuteDynamic(def.asOnDetach, Info->fdProcessInfo->dwProcessId, Info->fdProcessInfo->dwThreadId);
}

void DebugEventCallback(CBTYPE Type, PLUG_CB_DEBUGEVENT *Info)
{
}

void MenuEntryCallback(CBTYPE Type, PLUG_CB_MENUENTRY *Info)
{
	// void OnMenuEvent(int Entry)
	asExecuteDynamic(def.asOnMenuEvent, Info->hEntry);
}

DLL_EXPORT bool pluginit(PLUG_INITSTRUCT *InitStruct)
{
	InitStruct->pluginVersion	= PLUGIN_VERSION;
	InitStruct->sdkVersion		= PLUG_SDKVERSION;
	pluginHandle				= InitStruct->pluginHandle;
	strcpy_s(InitStruct->pluginName, PLUGIN_NAME);

	// Initialize the scripting engine
	char dir[MAX_PATH];
	GetCurrentDirectoryA(sizeof(dir), dir);
	strcat_s(dir, "\\test.as");
	
	if (!Script::EngineInit())
	{
		_plugin_printf("Script engine initialization failed!\n");
		return false;
	}

	Script::EngineLoad(&def, dir);

	// Add all of the callbacks
	_plugin_registercallback(pluginHandle, CB_INITDEBUG,			(CBPLUGIN)InitDebugCallback);
	_plugin_registercallback(pluginHandle, CB_STOPDEBUG,			(CBPLUGIN)StopDebugCallback);
	_plugin_registercallback(pluginHandle, CB_CREATEPROCESS,		(CBPLUGIN)CreateProcessCallback);
	_plugin_registercallback(pluginHandle, CB_EXITPROCESS,			(CBPLUGIN)ExitProcessCallback);
	_plugin_registercallback(pluginHandle, CB_CREATETHREAD,			(CBPLUGIN)CreateThreadCallback);
	_plugin_registercallback(pluginHandle, CB_EXITTHREAD,			(CBPLUGIN)ExitThreadCallback);
	_plugin_registercallback(pluginHandle, CB_SYSTEMBREAKPOINT,		(CBPLUGIN)SystemBreakpointCallback);
	_plugin_registercallback(pluginHandle, CB_LOADDLL,				(CBPLUGIN)LoadDllCallback);
	_plugin_registercallback(pluginHandle, CB_UNLOADDLL,			(CBPLUGIN)UnloadDllCallback);
	_plugin_registercallback(pluginHandle, CB_OUTPUTDEBUGSTRING,	(CBPLUGIN)DebugStringCallback);
	_plugin_registercallback(pluginHandle, CB_EXCEPTION,			(CBPLUGIN)ExceptionCallback);
	_plugin_registercallback(pluginHandle, CB_BREAKPOINT,			(CBPLUGIN)BreakpointCallback);
	_plugin_registercallback(pluginHandle, CB_PAUSEDEBUG,			(CBPLUGIN)PauseCallback);
	_plugin_registercallback(pluginHandle, CB_RESUMEDEBUG,			(CBPLUGIN)ResumeCallback);
	_plugin_registercallback(pluginHandle, CB_STEPPED,				(CBPLUGIN)SteppedCallback);
	_plugin_registercallback(pluginHandle, CB_ATTACH,				(CBPLUGIN)AttachCallback);
	_plugin_registercallback(pluginHandle, CB_DETACH,				(CBPLUGIN)DetachCallback);
	_plugin_registercallback(pluginHandle, CB_DEBUGEVENT,			(CBPLUGIN)DebugEventCallback);
	_plugin_registercallback(pluginHandle, CB_MENUENTRY,			(CBPLUGIN)MenuEntryCallback);

	return true;
}

DLL_EXPORT bool plugstop()
{
	_plugin_menuclear(hMenu);

	return true;
}

DLL_EXPORT void plugsetup(PLUG_SETUPSTRUCT *SetupStruct)
{
	hwndDlg = SetupStruct->hwndDlg;
	hMenu	= SetupStruct->hMenu;

	// Initialize the menu
	_plugin_menuaddentry(hMenu, 0, "Load script");

	// void OnPluginSetup(int DebuggerVersion, int PluginVersion)
	asExecuteDynamic(def.asOnPluginSetup, 20, PLUGIN_VERSION);
}